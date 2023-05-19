#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "requestmapper.h"
#include "global.h"

using namespace stefanfrings;

/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString fileName("webapp.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../fhir-gateway/etc"); // for development with shadow build (Linux)
    searchList.append(binDir+"/../../fhir-gateway/etc"); // for development with shadow build (Windows)
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}

/**
  Entry point of the program.
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString configFileName = searchConfigFile();

    // Configure logging
    QSettings* logSettings = new QSettings(configFileName, QSettings::IniFormat, &a);
    logSettings->beginGroup("logging");
    logger = new FileLogger(logSettings, 10000, &a);
    logger->installMsgHandler();

    // Log library version
    qDebug("QtWebApp has version %s", getQtWebAppLibVersion());

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&a);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&a);

    // Configure HTTP server listener
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &a);
    listenerSettings->beginGroup("listener");
    qDebug("config file loaded");

    // Start HTTP server
    new HttpListener(listenerSettings, new RequestMapper(&a), &a);

    return a.exec();
}

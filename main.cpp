#include <QCoreApplication>
#include <QSettings>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "requestmapper.h"
#include "global.h"

using namespace stefanfrings;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString configFileName = "../fhir-gateway/etc/webapp.ini";

    // Configure logging
    QSettings* logSettings = new QSettings(configFileName, QSettings::IniFormat, &a);
    logSettings->beginGroup("logging");
    logger = new FileLogger(logSettings, 10000, &a);
    logger->installMsgHandler();

    // Log library version
    qDebug("QtWebApp has version %s", getQtWebAppLibVersion());

    //Configure HTTP server listener
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &a);
    listenerSettings->beginGroup("listener");
    qDebug("config file loaded");

    //Start HTTP server
    new HttpListener(listenerSettings, new RequestMapper(&a), &a);

    return a.exec();
}

#include "getfilecontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>

GetFileController::GetFileController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();

}

void GetFileController::service(HttpRequest &request, HttpResponse &response){
    this->response=&response;

    QString path = rootdir + request.getPath();

    QFileInfo fi(path);
    if(fi.exists() && fi.isFile()) {
        getSingleFile(request, response);
    }
    else if(fi.exists() && fi.isDir()){
        getMultipleFiles(request, response);
    }
    else {
        response.setStatus(404, "Not Found");
        return;
    }
}

void GetFileController::getSingleFile(HttpRequest &request, HttpResponse &response){

    QString filename = request.getPath();
    QFile file(rootdir + filename);

    QString mime = QMimeDatabase().mimeTypeForFile(filename).name();

    response.setHeader("Content-Type", mime.toUtf8());

    if(!file.open(QIODevice::ReadOnly))
    {
        response.setStatus(404, "Not Found");
        return;
    }
    QByteArray blob = file.readAll();
    if(mime.contains("image")){
        blob.toBase64();
    }
    response.write(blob);
}

void GetFileController::getMultipleFiles(HttpRequest &request, HttpResponse &response){

    QString dirname = rootdir + request.getPath();
    QDir dir(dirname);

    QStringList images = dir.entryList();

    QJsonObject obj;
    response.setHeader("Content-Type", "application/json");

    int count = 0;
    foreach(QString filename, images){
        QFile file(dirname + "/" + filename);

        QString mime = QMimeDatabase().mimeTypeForFile(filename).name();

       if(mime!= "inode/directory"){

           if(!file.open(QIODevice::ReadOnly))
           {
               response.setStatus(404, "Not Found");
               return;
           }

           QByteArray blob = file.readAll();
           if(mime.contains("image")){
               blob.toBase64();
           }

           //response.write(blob);
           const QString image = blob.toBase64();
           obj.insert(filename, QJsonValue(image));

           if(++count == 10){
               break;
           }
       }
    }

    QJsonDocument jsonDoc;
    jsonDoc.setObject(obj);

    response.write(jsonDoc.toJson());
}

void GetFileController::finished(QNetworkReply *reply){
    QString url = reply->url().toString();
    QString ret = reply->readAll();

    response->write(ret.toUtf8());

    //    QJsonDocument doc = QJsonDocument::fromJson(ret.toUtf8());
    //    QJsonObject obj = doc.object();
    //    QJsonValue val = obj.value("text");
    //    QJsonObject textObj = val.toObject();
    //    QJsonValue val2 = textObj["div"];
    //    response->write("\n\n" + val2.toString().toUtf8());


}

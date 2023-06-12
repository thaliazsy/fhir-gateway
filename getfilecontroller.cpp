#include "getfilecontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QMimeDatabase>

GetFileController::GetFileController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();

}

void GetFileController::service(HttpRequest &request, HttpResponse &response){
    this->response=&response;

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

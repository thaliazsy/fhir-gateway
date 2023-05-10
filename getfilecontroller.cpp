#include "getfilecontroller.h"
#include <QEventLoop>
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

        QString token = request.getHeader("Authorization");

        //    if(token!="" && token.startsWith("Bearer ")){
        //        token = token.mid(7);
        //        QStringList jwt = token.split('.');
        //        QByteArray h = QByteArray::fromBase64(jwt[0].toUtf8());
        //        QByteArray p = QByteArray::fromBase64(jwt[1].toUtf8());
        //        QByteArray s = QByteArray::fromBase64(jwt[2].toUtf8());

        //        QJsonDocument doc = QJsonDocument::fromJson(h);
        //        QJsonObject header = doc.object();
        //        QJsonDocument payload = QJsonDocument::fromJson(p);

        //        //verify signature
        //        QByteArray key = "your-secret-key";
        //        QString alg = header["alg"].toString();
        //        if(alg=="HS256"){

        //        }
        //        response.write(h + "\n");
        //        response.write(p + "\n");
        //        response.write(s + "\n");

        //    }


        if(token!="")
        {
            QString filename = request.getPath();
            QFile file("/home/hapi2/Documents/" + filename);

            QString mime = QMimeDatabase().mimeTypeForFile(filename).name();

            response.setHeader("Content-Type", mime.toUtf8());
            //response.setHeader("Content-Type", "image/jpeg");

            if(!file.open(QIODevice::ReadOnly)) return;
            QByteArray blob = file.readAll();
            if(mime.contains("image")){
                blob.toBase64();
            }
            response.write(blob);

        }
        else {
            response.setStatus(401, "Authentication failed");
            response.write("The request cannot be processed because of failed authentication.", true);
        }

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

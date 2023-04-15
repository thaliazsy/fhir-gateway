#include "getfilecontroller.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

GetFileController::GetFileController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();

}

void GetFileController::service(HttpRequest &request, HttpResponse &response){
    this->request=&request;
    this->response=&response;    //Check token
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT");
    response.setHeader("Access-Control-Allow-Headers", "Authorization, Access-Control-Allow-Headers, Origin, Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers");

    if(isPreflight(request)){
        response.setStatus(200);
    }
    else {

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


        if(token=="Bearer 123")
        {
            response.setHeader("Content-Type", "application/pdf");
            QString filename = request.getPath();
            QFile file("/home/hapi2/Documents/" + filename);
            if(!file.open(QIODevice::ReadOnly)) return;
            QByteArray blob = file.readAll();
            response.write(blob);

        }
        else {
            response.setStatus(401, "Authentication failed");
            response.write("The request cannot be processed because of failed authentication.", true);
        }
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

#include "fhircontroller.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

FHIRController::FHIRController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();

}

QString base64UrlEncode(QByteArray ag){
    QString str = ag.toBase64(QByteArray::OmitTrailingEquals);
    str = str.replace("+", "-");
    str = str.replace("/", "_");
    return str;
}

QString base64UrlDecode(QByteArray ag){
    QByteArray str = ag.replace("-", "+");
    str = str.replace("_", "/");
    str = ag.fromBase64(str);
    return str;
}

void FHIRController::service(HttpRequest &request, HttpResponse &response){
    this->request=&request;
    this->response=&response;    //Check token
    QString token = request.getHeader("Authorization");
    if(token!="" && token.startsWith("Bearer ")){
        token = token.mid(7);
        QStringList jwt = token.split('.');
        QByteArray h = QByteArray::fromBase64(jwt[0].toUtf8());
        QByteArray p = QByteArray::fromBase64(jwt[1].toUtf8());
        QByteArray s = QByteArray::fromBase64(jwt[2].toUtf8());

        QJsonDocument doc = QJsonDocument::fromJson(h);
        QJsonObject header = doc.object();
        QJsonDocument payload = QJsonDocument::fromJson(p);

        //verify signature
        QByteArray key = "your-secret-key";
        QString alg = header["alg"].toString();
        if(alg=="HS256"){

        }
        response.write(h + "\n");
        response.write(p + "\n");
        response.write(s + "\n");

    }




    //if(token=="Bearer 123")
    //{
        //response.setHeader("Content-Type", "application/json");

        if(request.getMethod()=="GET"){
            QUrl url(baseURL + request.getPath());
            QNetworkRequest req(url);
            //req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *reply = m_manager->get(req);
            qDebug("fetching resource... ");
                    qDebug() << url.toString();

                    QEventLoop loop;
                    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                    //connect(m_manager, &QNetworkAccessManager::finished, this, &FHIRController::finished);
                    loop.exec();
                    finished(reply);
        }
        else if(request.getMethod()=="PUT"){
            QUrl url(baseURL + request.getPath());
            QNetworkRequest req(url);
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *reply = m_manager->put(req, request.getBody());
            qDebug("fetching resource... ");
                    qDebug() << url.toString();

                    QEventLoop loop;
                    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                    //connect(m_manager, &QNetworkAccessManager::finished, this, &FHIRController::finished);
                    loop.exec();
                    finished(reply);
        }
        else if(request.getMethod()=="POST"){
            QUrl url(baseURL + request.getPath());
            QNetworkRequest req(url);
            //req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *reply = m_manager->post(req, request.getBody());
            qDebug("fetching resource... ");
                    qDebug() << url.toString();

                    QEventLoop loop;
                    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                    //connect(m_manager, &QNetworkAccessManager::finished, this, &FHIRController::finished);
                    loop.exec();
                    finished(reply);
        }




//    }
//    else {
//        //response.setStatus(401, "Authentication failed");
//        response.write("The request cannot be processed because of failed authentication.", true);
//    }
}



void FHIRController::finished(QNetworkReply *reply){
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

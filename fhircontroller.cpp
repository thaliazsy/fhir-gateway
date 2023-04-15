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
    this->response=&response;
    response.setHeader("Access-Control-Allow-Origin", "*");
    response.setHeader("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT");
    response.setHeader("Access-Control-Allow-Headers", "Authorization, Access-Control-Allow-Headers, Origin, Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers");

    if(isPreflight(request)){
        response.setStatus(200);
    }
    else {
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

        if(token=="Bearer 123")
        {
            response.setHeader("Content-Type", "application/json");

            QUrl url(baseURL + request.getPath());
            QNetworkRequest req(url);
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *reply = m_manager->get(req);

            qDebug("fetching resource... ");
            qDebug() << url.toString();

            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            //connect(m_manager, &QNetworkAccessManager::finished, this, &FHIRController::finished);
            loop.exec();
            finished(reply);
        }
        else {
            response.setStatus(401, "Authentication failed");
            response.write("The request cannot be processed because of failed authentication.", true);
        }
    }
}



void FHIRController::finished(QNetworkReply *reply){
    QString url = reply->url().toString();
    QString ret = reply->readAll();
    response->write(url.toUtf8()+ "\n\n");
    response->write(ret.toUtf8());
}

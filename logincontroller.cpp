#include "logincontroller.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

LoginController::LoginController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();
}

void LoginController::service(HttpRequest &request, HttpResponse &response){
    //response.write("Login Test", true);

    this->request=&request;

    //this->response=&response;    //Check token

    QByteArray username = request.getParameter("username");
    QByteArray password = request.getParameter("password");

    response.setHeader("Content-Type", "application/json");

    QUrl url(baseURL + "/fhir/Person?identifier=" + username);
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


//    QString token = request.getHeader("Authorization");
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
}

void LoginController::finished(QNetworkReply *reply){
    QString url = reply->url().toString();
    QString ret = reply->readAll();
    response->write(url.toUtf8()+ "\n\n");
    response->write(ret.toUtf8());
}

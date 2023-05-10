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
    this->response=&response;

    response.setHeader("Content-Type", "application/json");
    QUrl url(baseURL + request.getPath());
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nullptr;

    if(request.getMethod()=="GET"){
        reply = m_manager->get(req);
    }
    else if(request.getMethod()=="PUT"){
        reply = m_manager->put(req, request.getBody());
    }
    else if(request.getMethod()=="POST"){
        reply = m_manager->post(req, request.getBody());
    }

    qDebug("fetching resource... ");
    qDebug() << url.toString();

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //connect(m_manager, &QNetworkAccessManager::finished, this, &FHIRController::finished);
    loop.exec();
    finished(reply);
}

void FHIRController::finished(QNetworkReply *reply){
    QString ret = reply->readAll();
    response->write(ret.toUtf8(), true);
}

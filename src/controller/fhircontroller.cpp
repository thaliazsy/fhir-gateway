#include "fhircontroller.h"

FHIRController::FHIRController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();
}



void FHIRController::service(HttpRequest &request, HttpResponse &response) {
    this->response=&response;

    response.setHeader("Content-Type", "application/json");

    QByteArray params = getParameters(request);

    // Send request
    QUrl url(baseURL + request.getPath() + params);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nullptr;

    if (request.getMethod() == "GET") {
        reply = m_manager->get(req);
    }
    else if (request.getMethod() == "PUT") {
        reply = m_manager->put(req, request.getBody());
    }
    else if (request.getMethod() == "POST") {
        reply = m_manager->post(req, request.getBody());
    }

    qDebug("fetching resource... ");
    qDebug() << url.toString();

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    finished(reply);
}

void FHIRController::finished(QNetworkReply *reply){
    QString ret = reply->readAll();
    response->write(ret.toUtf8(), true);
}

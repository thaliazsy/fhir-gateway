#ifndef FHIRCONTROLLER_H
#define FHIRCONTROLLER_H

#include "httprequesthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

using namespace stefanfrings;

class FHIRController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QString baseURL = "http://localhost:8080";
    HttpRequest* request;
    HttpResponse* response;
    FHIRController(QObject* parent=0);
    QNetworkAccessManager *m_manager;
    QNetworkRequest req;
    void service(HttpRequest& request, HttpResponse& response);
    void finished(QNetworkReply* reply);
};

#endif // FHIRCONTROLLER_H

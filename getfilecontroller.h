#ifndef GETFILECONTROLLER_H
#define GETFILECONTROLLER_H

#include "global.h"
#include "httprequesthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace stefanfrings;

class GetFileController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QString baseURL = "http://localhost:8080";
    HttpRequest* request;
    HttpResponse* response;
    GetFileController(QObject* parent=0);
    QNetworkAccessManager *m_manager;
    QNetworkRequest req;
    //bool isPreflight(HttpRequest &request);
    void service(HttpRequest& request, HttpResponse& response);
    void finished(QNetworkReply* reply);
};

#endif // GETFILECONTROLLER_H

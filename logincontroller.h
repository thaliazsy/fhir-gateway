#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include "httprequesthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace stefanfrings;

class LoginController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QString baseURL = "http://203.64.84.32:9876";
    HttpRequest* request;
    HttpResponse* response;
    LoginController(QObject* parent=0);
    QNetworkAccessManager *m_manager;
    QNetworkRequest req;
    void service(HttpRequest& request, HttpResponse& response);
    void finished(QNetworkReply* reply);
};

#endif // LOGINCONTROLLER_H

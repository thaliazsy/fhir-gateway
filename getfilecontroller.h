#ifndef GETFILECONTROLLER_H
#define GETFILECONTROLLER_H

#include "httprequesthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

using namespace stefanfrings;

class GetFileController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QByteArray rootdir = "/home/hapi2/Documents";
    HttpRequest* request;
    HttpResponse* response;
    GetFileController(QObject* parent=0);
    QNetworkAccessManager *m_manager;
    QNetworkRequest req;
    void service(HttpRequest& request, HttpResponse& response);
    void finished(QNetworkReply* reply);
};

#endif // GETFILECONTROLLER_H

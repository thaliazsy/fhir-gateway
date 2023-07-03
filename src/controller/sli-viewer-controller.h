#ifndef SLI-VIEWER-CONTROLLER_H
#define SLI-VIEWER-CONTROLLER_H

#include "../global.h"
#include "httprequesthandler.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

using namespace stefanfrings;

class SLIViewerController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QByteArray baseURL = "http://localhost:5000";
    HttpRequest* request;
    HttpResponse* response;
    SLIViewerController(QObject* parent=0);
    QNetworkAccessManager *m_manager;
    QNetworkRequest req;
    void service(HttpRequest& request, HttpResponse& response);
    void finished(QNetworkReply* reply);
};

#endif // SLI-VIEWER-CONTROLLER_H

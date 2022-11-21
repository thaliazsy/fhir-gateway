#ifndef UPLOADCONTROLLER_H
#define UPLOADCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class UploadController: public HttpRequestHandler
{
    Q_OBJECT
public:
    UploadController(QObject* parent=0);
    HttpRequest* request;
    HttpResponse* response;
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // UPLOADCONTROLLER_H

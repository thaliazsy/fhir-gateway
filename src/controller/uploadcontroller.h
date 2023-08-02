#ifndef UPLOADCONTROLLER_H
#define UPLOADCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class UploadController: public HttpRequestHandler
{
    Q_OBJECT
public:
    QByteArray rootdir = "/home/hapi2/Documents/getfile/skin-lesion/images/";
    QByteArray outpath = "http://repo.tcumi.com:9876/getfile/skin-lesion/images/";
    UploadController(QObject* parent=0);
    HttpRequest* request;
    HttpResponse* response;
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // UPLOADCONTROLLER_H

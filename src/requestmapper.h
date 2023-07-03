#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"

// API Requests
#include "controller/helloworldcontroller.h"
#include "controller/uploadcontroller.h"
#include "controller/fhircontroller.h"
#include "controller/getfilecontroller.h"
#include "controller/sli-viewer-controller.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler
{
    Q_OBJECT
public:
    RequestMapper(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
    bool isPreflight(HttpRequest &request);
    bool verifyToken(HttpRequest &request);
};

#endif // REQUESTMAPPER_H

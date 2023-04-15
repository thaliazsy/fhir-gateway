#include "requestmapper.h"
#include "helloworldcontroller.h"
#include "uploadcontroller.h"
#include "fhircontroller.h"
#include "getfilecontroller.h"

RequestMapper::RequestMapper(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s", path.data());

    if(path=="/" || path=="/hello") {
        HelloWorldController().service(request, response);
    }
    else if(path=="/upload") {
        UploadController().service(request, response);
    }
    else if(path.startsWith("/fhir")) {
        FHIRController().service(request, response);
    }
    else if(path.startsWith("/getfile")) {
        GetFileController().service(request, response);
    }
    else {
        response.setStatus(404, "Not Found");
        response.write("The URL is wrong, no such document.", true);
    }
    qDebug("RequestMapper: finished request");
}

#include "requestmapper.h"
#include <QJsonDocument>
#include <QJsonObject>

RequestMapper::RequestMapper(QObject* parent)
    : HttpRequestHandler(parent)
{

}

bool RequestMapper::isPreflight(HttpRequest &request)
{
    if(request.getMethod()=="OPTIONS"){
        QString method = request.getHeader("Access-Control-Request-Method");
        QString headers = request.getHeader("Access-Control-Request-Headers");
        QString origin = request.getHeader("Origin");
        if(method!="" && headers!="" && origin!=""){
            return true;
        }
    }
    return false;
}

bool RequestMapper::verifyToken(HttpRequest &request)
{
    //QString token = "123";
    QString token = "";
    QByteArray referer = request.getHeader("Referer");
    QByteArray path = request.getPath();

    if (referer.contains("skinlesionimage-ms")) {
           return true;
    }
    else if (path.startsWith("/skinlesionimage-ms")) {
        token = request.getParameter("Authorization");
    }
    else {
        token = request.getHeader("Authorization");
    }

//            if(token!="" && token.startsWith("Bearer ")){
//                token = token.mid(7);
//                QStringList jwt = token.split('.');
//                QByteArray h = QByteArray::fromBase64(jwt[0].toUtf8());
//                QByteArray p = QByteArray::fromBase64(jwt[1].toUtf8());
//                QByteArray s = QByteArray::fromBase64(jwt[2].toUtf8());

//                QJsonDocument doc = QJsonDocument::fromJson(h);
//                QJsonObject header = doc.object();
//                QJsonDocument payload = QJsonDocument::fromJson(p);

//                //verify signature
//                QByteArray key = "H707-tzuchiuniverisity-masterprogram-110";
//                QString alg = header["alg"].toString();
//                if(alg=="HS256"){

//                }
//            }

    return (token != "");
}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    QByteArray referer = request.getHeader("Referer");
    qDebug("RequestMapper: path=%s", path.data());

    response.setHeader("Access-Control-Allow-Origin", "*");

    if(isPreflight(request)){
        qDebug("RequestMapper: preflight request");
        response.setHeader("Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT");
        response.setHeader("Access-Control-Allow-Headers", "Authorization, Access-Control-Allow-Headers, Origin, Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers");
        response.setStatus(200);
        return;
    }

    //Verify Token
    if(!verifyToken(request)){
        response.setStatus(401, "Authentication failed");
        response.write("The request cannot be processed because of failed authentication.", true);
        return;
    }
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
    else if(path.startsWith("/skinlesionimage-ms") || referer.contains("skinlesionimage-ms")){
        SLIViewerController().service(request, response);
    }
    else if(path.startsWith("/skin-lesion-viewer")){

        response.write("<html><body>");
        response.write("<h1>This is Viewer</h1>");
        response.write("<p>Access Token: " + request.getParameter("Authorization") + "</p>");
        response.write("<p>Document URL: " + request.getParameter("docUrl") + "</p>");
        response.write("</body></hhtml>");
        //dynamically generate js
        //
        //staticFileController->service(request, response);
    }
    else {
        response.setStatus(404, "Not Found");
        response.write("The URL is wrong, no such document.", true);
    }
    qDebug("RequestMapper: finished request");
}

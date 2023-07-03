#include "sli-viewer-controller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QNetworkCookie>

SLIViewerController::SLIViewerController(QObject* parent)
    : HttpRequestHandler(parent)
{
    m_manager = new QNetworkAccessManager();
}

void getLocalFiles(HttpRequest &request, HttpResponse &response){
    QString filename = request.getPath();
    QFile file("/home/hapi2/Documents/SkinLesionManagementSystemPublish/wwwroot" + filename);

    QString mime = QMimeDatabase().mimeTypeForFile(filename).name();

    response.setHeader("Content-Type", mime.toUtf8());

    if(!file.open(QIODevice::ReadOnly)) {
        response.setStatus(404, "Not Found");
        return;
    }

    QByteArray blob = file.readAll();
    if(mime.contains("image")){
        blob.toBase64();
    }
    response.write(blob);
}

void SLIViewerController::service(HttpRequest &request, HttpResponse &response){
    this->response=&response;

    QByteArray path=request.getPath();
    QByteArray referer = request.getHeader("Referer");

    if(path.startsWith("/skinlesionimage-ms")) {
        QNetworkReply *reply = nullptr;

        QByteArray params = getParameters(request);

        if(referer.contains("skinlesionimage-ms")) {

            QUrl url(baseURL + path + params);

            QNetworkRequest req(url);

            //Get Cookies
            QMapIterator<QByteArray,QByteArray> cookies(request.getCookieMap());
            QList<QNetworkCookie> cookieJar;
            while(cookies.hasNext()){
                cookies.next();
                cookieJar.append(QNetworkCookie(cookies.key(), cookies.value()));
                QByteArray cookieStr = cookies.key() + "=" + cookies.value()+";";
            }
            req.setHeader(QNetworkRequest::KnownHeaders::CookieHeader, QVariant::fromValue(cookieJar));

            if(request.getMethod()=="GET"){
                reply = m_manager->get(req);
            }
            else if(request.getMethod()=="PUT"){
                reply = m_manager->put(req, request.getBody());
            }
            else if(request.getMethod()=="POST"){
                req.setHeader(QNetworkRequest::ContentTypeHeader, request.getHeader("Content-Type"));
                reply = m_manager->post(req, request.getBody());
            }
        }
        else {
            QByteArray params = "?documentbundle=" + request.getParameter("documentbundle");
            QUrl url(baseURL + path + params);
            QNetworkRequest req(url);
            reply = m_manager->get(req);
        }

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        finished(reply);
    }
    else {
        getLocalFiles(request, response);
    }
}

void SLIViewerController::finished(QNetworkReply *reply){
    if(reply->error()) {
        response->write(reply->errorString().toUtf8(), true);
    }
    else {
        QList<QNetworkReply::RawHeaderPair> pairs = reply->rawHeaderPairs();

        QByteArray cookieStr = reply->rawHeader("Set-Cookie");
        if(cookieStr!=""){
            QList<QByteArray> cookieJar = cookieStr.split('\n');
            foreach(QByteArray cookie, cookieJar){
                response->setCookie(HttpCookie(cookie));
            }
        }

        foreach (QNetworkReply::RawHeaderPair pair, pairs) {
            if(pair.first != "Transfer-Encoding" && pair.first!="Set-Cookie"){
                response->setHeader(pair.first, pair.second);
            }
        }

        QString ret = reply->readAll();
        response->write(ret.toUtf8(), true);
    }
}

#include "sli-viewer-controller.h"
#include "qmimedatabase.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMimeDatabase>

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
    this->request=&request;

    QByteArray path = request.getPath();
    if(path.startsWith("/ReportCreator")){
        QByteArray params = "?documentbundle=" + request.getParameter("documentbundle");

        QUrl url(baseURL + request.getPath() + params);

        qDebug("fetching resource... ");
        qDebug() << url.toString();

        QNetworkRequest req(url);

        QNetworkReply *reply = nullptr;

        reply = m_manager->get(req);

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
    QString ret = reply->readAll();
    response->write(ret.toUtf8(), true);
}

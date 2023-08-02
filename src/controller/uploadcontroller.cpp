#include "uploadcontroller.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

UploadController::UploadController(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void UploadController::service(HttpRequest &request, HttpResponse &response){

    this->request=&request;
    this->response=&response;
    QByteArray encounter = request.getParameter("encounter-id");
    QByteArray filename = request.getParameter("file-input");
    QTemporaryFile* tempFile=request.getUploadedFile("file-input");

    QJsonObject jsonObj;

    if (tempFile)
    {
        QString path = rootdir + encounter;
        QDir dir(path);

        if(!dir.exists()){
            dir.mkpath(".");
        }
        QFile file(path + "/" + filename);
        if(file.open(QIODevice::WriteOnly)) {
            while (!tempFile->atEnd() && !tempFile->error())
            {
                QByteArray buffer=tempFile->read(65536);
                file.write(buffer);
            }

            QString outStr(outpath + encounter + "/" + filename);
            jsonObj.insert("url", outStr);
        }
    }
    else
    {
        response.setStatus(400);
        jsonObj.insert("error", "Upload failed. \"image-input\" not found");

    }
    QJsonDocument doc(jsonObj);
    response.write(doc.toJson(), true);
}

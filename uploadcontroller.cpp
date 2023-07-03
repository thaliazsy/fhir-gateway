#include "uploadcontroller.h"
#include <QDir>

UploadController::UploadController(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void UploadController::service(HttpRequest &request, HttpResponse &response){

    this->request=&request;
    this->response=&response;
    QByteArray encounter = request.getParameter("encounter-name");
    QByteArray filename = request.getParameter("file-name");
    QTemporaryFile* tempFile=request.getUploadedFile("image-input");
    if (tempFile)
    {
        QString path = rootdir + encounter;
        QDir dir(path);

        if(!dir.exists()){
            dir.mkpath(".");
        }
        QFile file(path + "/" + filename);
        if(file.open(QIODevice::WriteOnly)){
            while (!tempFile->atEnd() && !tempFile->error())
            {
                QByteArray buffer=tempFile->read(65536);
                file.write(buffer);
            }
            response.write(outpath + encounter + "/" + filename);
        }
        else {
               response.write("Upload failed. \"file-name\" not found.", true);
        }
    }
    else
    {
        response.write("Upload failed. \"image-input\" not found.", true);
    }
}

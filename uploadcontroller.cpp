#include "uploadcontroller.h"

UploadController::UploadController(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void UploadController::service(HttpRequest &request, HttpResponse &response){

    this->request=&request;
    this->response=&response;
    QByteArray filename = request.getParameter("file-name");
    QTemporaryFile* tempFile=request.getUploadedFile("image-input");
    if (tempFile)
    {
        QFile file(rootdir + filename);
        if(file.open(QIODevice::WriteOnly)){
            while (!tempFile->atEnd() && !tempFile->error())
            {
                QByteArray buffer=tempFile->read(65536);
                file.write(buffer);
            }
            response.write(outpath + filename);
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

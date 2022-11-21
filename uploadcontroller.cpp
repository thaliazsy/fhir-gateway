#include "uploadcontroller.h"

UploadController::UploadController(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void UploadController::service(HttpRequest &request, HttpResponse &response){

    this->request=&request;
    this->response=&response;    //Check token
    if(true)
    {
        QByteArray img = request.getParameter("image-input");
        QByteArray imageData = QByteArray::fromBase64(img);
    }
    else
    //request.getParameter("action")=="show")
        {
            response.setHeader("Content-Type", "image/jpeg");
            QTemporaryFile* file=request.getUploadedFile("image-input");
            if (file)
            {
                while (!file->atEnd() && !file->error())
                {
                    QByteArray buffer=file->read(65536);
                    response.write(buffer);
                }
            }
            else
            {
                response.write("upload failed");
            }
        }

}

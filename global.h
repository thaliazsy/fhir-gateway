#ifndef GLOBAL_H
#define GLOBAL_H

#include "staticfilecontroller.h"
#include "filelogger.h"

using namespace stefanfrings;

/** Controller for static files */
extern  StaticFileController* staticFileController;

/** Redirects log messages to a file **/
extern FileLogger* logger;

/** Get parameter from request **/
QByteArray getParameters(HttpRequest &request);

/** Get cookies from request **/

/** Redirect cookies to local request **/

QString base64UrlEncode(QByteArray ag);
QString base64UrlDecode(QByteArray ag);


#endif // GLOBAL_H

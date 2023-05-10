#ifndef GLOBAL_H
#define GLOBAL_H

#include "staticfilecontroller.h"
#include "filelogger.h"

using namespace stefanfrings;

/** Controller for static files */
extern  StaticFileController* staticFileController;

/** Redirects log messages to a file **/
extern FileLogger* logger;


#endif // GLOBAL_H

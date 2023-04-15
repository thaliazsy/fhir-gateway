#include "global.h"

FileLogger* logger;

bool isPreflight(HttpRequest &request){
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

#include "global.h"

StaticFileController* staticFileController;
FileLogger* logger;

QByteArray getParameters(HttpRequest &request){
    QMultiMapIterator<QByteArray, QByteArray> i(request.getParameterMap());
    QByteArray params = i.hasNext() ? "?" : "";

    while (i.hasNext()){
        params+= i.hasPrevious() ? "&" : "";
        i.next();
        params+= i.key()+ "=" + i.value();
    }
    return params;
}

QString base64UrlEncode(QByteArray ag) {
    QString str = ag.toBase64(QByteArray::OmitTrailingEquals);
    str = str.replace("+", "-");
    str = str.replace("/", "_");
    return str;
}

QString base64UrlDecode(QByteArray ag) {
    QByteArray str = ag.replace("-", "+");
    str = str.replace("_", "/");
    str = ag.fromBase64(str);
    return str;
}

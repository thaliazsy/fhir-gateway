QT -= gui
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        fhircontroller.cpp \
        getfilecontroller.cpp \
        global.cpp \
        helloworldcontroller.cpp \
        main.cpp \
        requestmapper.cpp \
        uploadcontroller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    etc/webapp.ini

include(../../QtWebApp/QtWebApp/httpserver/httpserver.pri)
include(../../QtWebApp/QtWebApp/logging/logging.pri)

HEADERS += \
    fhircontroller.h \
    getfilecontroller.h \
    global.h \
    helloworldcontroller.h \
    requestmapper.h \
    uploadcontroller.h

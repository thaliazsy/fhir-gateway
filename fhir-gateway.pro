QT -= gui
QT += core
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/global.cpp \
        src/main.cpp \
        src/requestmapper.cpp \
        src/controller/fhircontroller.cpp \
        src/controller/getfilecontroller.cpp \
        src/controller/helloworldcontroller.cpp \
        src/controller/sli-viewer-controller.cpp \
        src/controller/uploadcontroller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    etc/webapp.ini

include(httpserver/httpserver.pri)
include(logging/logging.pri)

HEADERS += \
    src/global.h \
    src/requestmapper.h \
    src/controller/fhircontroller.h \
    src/controller/getfilecontroller.h \
    src/controller/helloworldcontroller.h \
    src/controller/sli-viewer-controller.h \
    src/controller/uploadcontroller.h

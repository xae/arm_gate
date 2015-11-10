#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T13:49:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = armhttpserver
TEMPLATE = app

CONFIG(debug, debug|release):{OBJECTS_DIR = $$PWD/../../../build/armhttpserver/debug/obj}
else:{OBJECTS_DIR = $$PWD/../../../build/armhttpserver/release/obj}
RCC_DIR = $$PWD/../../../build/armhttpserver/rcc
UI_DIR = $$PWD/../../../build/armhttpserver/ui
MOC_DIR = $$PWD/../../../build/armhttpserver/moc
DESTDIR = $$PWD/../../../app

SOURCES += main.cpp\
    serverimpl.cpp \
    serverrunner.cpp

HEADERS  += \
    serverimpl.h \
    serverrunner.h \
    fileinfo.h

FORMS    +=

libname = qhttpserver

LIBS += -L$$PWD/../qhttpserver/lib/

INCLUDEPATH += $$PWD/../qhttpserver/src

CONFIG(debug, debug|release): {
    LIBS += $$join(libname,,'-l',d)
    libname = $$join(libname,,,d)

} else: {
    LIBS += $$join(libname,,'-l',)

}
    message($$libname)
    message($$LIBS)

libfiles.path = $$DESTDIR
libfiles.files = $$join(libname,,$$PWD/../qhttpserver/lib/,.*)
INSTALLS += libfiles

RESOURCES += \
    armhttpserver.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T14:21:18
#
#-------------------------------------------------

QT       += network

QT       -= gui

TEMPLATE = lib

DEFINES += SRC_LIBRARY

DESTDIR = $$PWD/../bin

include(src.pri)
include(../../AbstractAsyncClient/abstractasyncclient_lib.pri)

greaterThan(QT_MAJOR_VERSION, 4): {
    TARGET = hardwareserver5
    CONFIG += C++11
} else {
    TARGET = hardwareserver
    QMAKE_CXXFLAGS += -std=c++11
}

CONFIG(debug, debug|release): {
    OBJECTS_DIR = $$PWD/../build/lib/debug/obj/qt_ver_
    TARGET = $$join(TARGET,,,d)
}else: {
    OBJECTS_DIR = $$PWD/../build/lib/release/obj/qt_ver_
}
RCC_DIR = $$PWD/../build/lib/rcc/qt_ver_
UI_DIR = $$PWD/../build/lib/ui/qt_ver_
MOC_DIR = $$PWD/../build/lib/moc/qt_ver_

OBJECTS_DIR = $$replace(OBJECTS_DIR,_ver_,$$QT_MAJOR_VERSION)
RCC_DIR = $$replace(RCC_DIR,_ver_,$$QT_MAJOR_VERSION)
UI_DIR = $$replace(UI_DIR,_ver_,$$QT_MAJOR_VERSION)
MOC_DIR = $$replace(MOC_DIR,_ver_,$$QT_MAJOR_VERSION)

unix {
    #target.path = /usr/lib
    #INSTALLS += target
} else {
    target.path = $$PWD/../../../app
    INSTALLS += target
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}






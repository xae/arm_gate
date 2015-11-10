#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T15:24:46
#
#-------------------------------------------------

QT       += core gui network


DESTDIR = $$PWD/../bin/testapp

greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets
    CONFIG += C++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}

include($$PWD/../hardwareserver_lib.pri)

CONFIG(debug, debug|release): {
    OBJECTS_DIR = $$PWD/../build/testapp/debug/obj/qt_ver_
    TARGET = $$join(TARGET,,,d)
}else: {
    OBJECTS_DIR = $$PWD/../build/testapp/release/obj/qt_ver_
}
RCC_DIR = $$PWD/../build/testapp/rcc/qt_ver_
UI_DIR = $$PWD/../build/testapp/ui/qt_ver_
MOC_DIR = $$PWD/../build/testapp/moc/qt_ver_

OBJECTS_DIR = $$replace(OBJECTS_DIR,_ver_,$$QT_MAJOR_VERSION)
RCC_DIR = $$replace(RCC_DIR,_ver_,$$QT_MAJOR_VERSION)
UI_DIR = $$replace(UI_DIR,_ver_,$$QT_MAJOR_VERSION)
MOC_DIR = $$replace(MOC_DIR,_ver_,$$QT_MAJOR_VERSION)

TARGET = HardwareServerTestApp
TEMPLATE = app


SOURCES += main.cpp\
        hardwareservertestappmainwindow.cpp \
    server.cpp \
    serverthreadworker.cpp

HEADERS  += hardwareservertestappmainwindow.h \
    hardwareinfo.h \
    server.h \
    serverthreadworker.h

FORMS    += hardwareservertestappmainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2014-11-17T17:47:06
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = AbstractAsyncClient
TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4): {
    TARGET = abstractasyncclient5
} else {
    TARGET = abstractasyncclient
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
DESTDIR = $$PWD/../bin

OBJECTS_DIR = $$replace(OBJECTS_DIR,_ver_,$$QT_MAJOR_VERSION)
RCC_DIR = $$replace(RCC_DIR,_ver_,$$QT_MAJOR_VERSION)
UI_DIR = $$replace(UI_DIR,_ver_,$$QT_MAJOR_VERSION)
MOC_DIR = $$replace(MOC_DIR,_ver_,$$QT_MAJOR_VERSION)

QMAKE_CXXFLAGS += -std=c++11

include(src.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
} else {
    target.path = $$PWD/../../../app
    INSTALLS += target
}

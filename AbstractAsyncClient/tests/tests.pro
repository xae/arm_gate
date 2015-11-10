#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T11:34:24
#
#-------------------------------------------------

QT       += testlib network

QT       -= gui

TARGET = tst_payloadpacket
CONFIG   += console
CONFIG   -= app_bundle

CONFIG(debug, debug|release):{OBJECTS_DIR = $$PWD/../build/tests/debug/obj}
else:{OBJECTS_DIR = $$PWD/../build/tests/release/obj}
RCC_DIR = $$PWD/../build/tests/rcc
UI_DIR = $$PWD/../build/tests/ui
MOC_DIR = $$PWD/../build/tests/moc
DESTDIR = $$PWD/../bin

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

include(../src/src.pri)

QMAKE_CXXFLAGS += -I$$PWD/../src/

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    main.cpp \
    simplecommand.cpp \
    simpletransport.cpp \
    testabstractasyncclient.cpp \
    testpayloadpacket.cpp

HEADERS += \
    simplecommand.h \
    simpletransport.h \
    testabstractasyncclient.h \
    testpayloadpacket.h

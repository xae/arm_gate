TARGET = helloworld

QT += network
QT -= gui

#CONFIG += debug

INCLUDEPATH += ../../src
LIBS += -L../../lib

CONFIG(debug, debug|release):{LIBS += -lqhttpserverd}
else:{LIBS += -lqhttpserver}

SOURCES = helloworld.cpp
HEADERS = helloworld.h

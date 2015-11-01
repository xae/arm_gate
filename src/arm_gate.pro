########################################################
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = arm_gate
TEMPLATE = app

########################################################
ROOT_DIR = $$PWD/../
BUILD_DIR = $$ROOT_DIR/build/$$TARGET/
CONFIG(debug, debug|release): {
    TARGET = "$${TARGET}d"
    TYPE_BUILD="debug"
} else: {
    TYPE_BUILD="release"
}

APP_DIR = $$ROOT_DIR/bin/
SRC_DIR = "$$ROOT_DIR/src/"
OUTPUT_DIR = "$$BUILD_DIR/$$TYPE_BUILD/"
OBJECTS_DIR = "$$OUTPUT_DIR/obj/"
MOC_DIR = "$$OUTPUT_DIR/moc/"
RCC_DIR = "$$OUTPUT_DIR/res/"
UI_DIR = "$$OUTPUT_DIR/ui/"
DESTDIR = "$$OUTPUT_DIR/bin/"

##################################################
VERSION = 0.02
DEFINES += MODULE_NAME=$$TARGET
DEFINES += VERSION_NUMBER=v$$VERSION
QMAKE_CXXFLAGS += -std=c++11


##################################################
### этап install
#PROJECT.path = "$$APP_DIR/"
#PROJECT.files += $$files($$DESTDIR/*.exe)
#PROJECT.files += $$files($$BUILD_DIR/../lib/$$TYPE_BUILD/lib/*.dll)
#INSTALLS += PROJECT

########################################################
INCLUDEPATH += "$$PWD"
INCLUDEPATH += "$$SRC_DIR"

SOURCES += main.cpp\
        mainwindow.cpp \
    arms_manager.cpp \
    remote_devices_manager.cpp \
    tcp_server.cpp

HEADERS  += mainwindow.h \
    arms_manager.h \
    remote_devices_manager.h \
    tcp_server.h

FORMS    += mainwindow.ui

RESOURCES += \
    res/arm_gate.qrc

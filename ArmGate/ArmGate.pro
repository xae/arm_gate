########################################################
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets
    CONFIG += C++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}
TARGET = ArmGate
TEMPLATE = app

########################################################
ROOT_DIR = $$PWD/
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
DESTDIR = "$$APP_DIR"

##################################################
VERSION = 0.04
DEFINES += MODULE_NAME=$$TARGET
DEFINES += VERSION_NUMBER=v$$VERSION

########################################################
include($$PWD/../HardwareServerLib/hardwareserver_lib.pri)
LIBS += -L$$PWD/../HardwareServerLib/bin

########################################################
INCLUDEPATH += "$$PWD"
INCLUDEPATH += "$$SRC_DIR"

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/arms_manager.cpp \
    src/remote_devices_manager.cpp

HEADERS  += src/mainwindow.h \
    src/arms_manager.h \
    src/remote_devices_manager.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    src/res/arm_gate.qrc

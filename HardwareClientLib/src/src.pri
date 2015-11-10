INCLUDEPATH += $$PWD/../include
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/hardwareclient.cpp \
    $$PWD/hardwareclientprivate.cpp \
    $$PWD/changenotifystatysincomingcommand.cpp \
    $$PWD/changenotifystatysincomingcommandimpl.cpp \
    $$PWD/changenotifystatepackethandler.cpp \
    $$PWD/changehardwarestatepackethandler.cpp \
    $$PWD/changehardwarestatusincomingcommand.cpp \
    $$PWD/changehardwarestatusincomingcommandimpl.cpp \
    $$PWD/startnotifyoutgoingcommand.cpp \
    $$PWD/armhardwareclientcommand.cpp \
    $$PWD/requesthardwarestatusoutgoingcommand.cpp \
    $$PWD/incommingmessagepackethandler.cpp \
    $$PWD/messageincomingcommand.cpp \
    $$PWD/messageincomingcommandimpl.cpp

HEADERS += \
    $$PWD/../include/hardwareclient.h \
    $$PWD/../include/ioutgoingcommand.h \
    $$PWD/../include/changenotifystatysincomingcommand.h \
    $$PWD/../include/changehardwarestatusincomingcommand.h \
    $$PWD/../include/iincomingcommand.h \
    $$PWD/../include/startnotifyoutgoingcommand.h \
    $$PWD/../include/requesthardwarestatusoutgoingcommand.h \
    $$PWD/../include/messageincomingcommand.h \
    $$PWD/../include/hardwareclient_global.h \
    $$PWD/changenotifystatysincomingcommandimpl.h \
    $$PWD/changenotifystatepackethandler.h \
    $$PWD/changehardwarestatepackethandler.h \
    $$PWD/changehardwarestatusincomingcommandimpl.h \
    $$PWD/iincomingpackethandler.h \
    $$PWD/armhardwareclientcommand.h \
    $$PWD/hardwareclientprivate.h \
    $$PWD/incommingmessagepackethandler.h \
    $$PWD/messageincomingcommandimpl.h

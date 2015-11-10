INCLUDEPATH += $$PWD/../include

SOURCES += \
    $$PWD/hardwareserver.cpp \
    $$PWD/hardwareserverprivate.cpp \
    $$PWD/startnotifyincomingcommand.cpp \
    $$PWD/startnotifyincomingcommandimpl.cpp \
    $$PWD/startnotifyincomingcommandthandler.cpp \
    $$PWD/requesthardwarestatusincomingcommand.cpp \
    $$PWD/requesthardwarestatusincomingcommandimpl.cpp \
    $$PWD/requesthardwarestatusincomingcommandhandler.cpp \
    $$PWD/outgoingservercommand.cpp \
    $$PWD/notifystatuschangeoutgoingcommand.cpp \
    $$PWD/hardwarestatuschangeoutgoingcommand.cpp \
    $$PWD/messageoutgoingcommand.cpp


HEADERS += \
    $$PWD/hardwareserverprivate.h \
    $$PWD/iincomingpackethandler.h \
    $$PWD/startnotifyincomingcommandimpl.h \
    $$PWD/startnotifyincomingcommandthandler.h \
    $$PWD/requesthardwarestatusincomingcommandimpl.h \
    $$PWD/requesthardwarestatusincomingcommandhandler.h \
    $$PWD/outgoingservercommand.h \
    $$PWD/../include/iincomingcommand.h \
    $$PWD/../include/hardwareserver.h\
    $$PWD/../include/startnotifyincomingcommand.h \
    $$PWD/../include/ioutgoingcommand.h \
    $$PWD/../include/requesthardwarestatusincomingcommand.h \
    $$PWD/../include/notifystatuschangeoutgoingcommand.h \
    $$PWD/../include/hardwarestatuschangeoutgoingcommand.h \
    $$PWD/../include/messageoutgoingcommand.h \
    $$PWD/../include/hardwareserver_global.h


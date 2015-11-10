DEFINES += ABSTRACTASYNCCLIENT_LIBRARY

QMAKE_CXXFLAGS += \
    -I$$PWD/../include/ \
    -I$$PWD


SOURCES += $$PWD/abstractasyncclient.cpp \
    $$PWD/payloadpacket.cpp \
    $$PWD/abstractasyncclientprivate.cpp \
    $$PWD/tcptransport.cpp \
    $$PWD/tcpasyncserver.cpp

HEADERS += $$PWD/../include/abstractasyncclient.h\
        $$PWD/../include/abstractasyncclient_global.h \
    $$PWD/../include/iabstracttransport.h \
    $$PWD/../include/icommand.h \
    $$PWD/../include/payloadpacket.h \
    $$PWD/abstractasyncclientprivate.h \
    $$PWD/../include/tcptransport.h \
    $$PWD/../include/tcpasyncserver.h \
    $$PWD/tcpasyncserverprivate.h

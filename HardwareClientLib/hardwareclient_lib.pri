QMAKE_CXXFLAGS += \
    -I$$PWD/include/

LIBS += -L$$PWD/bin

libnamehc = hardwareclient

greaterThan(QT_MAJOR_VERSION, 4): {
    lib = -lhardwareclient5
    libnamehc = $$join(libname,,,5)
} else {
    lib = -lhardwareclient
}

CONFIG(debug, debug|release): {
    LIBS += $$join(lib,,,d)
    libnamehc = $$join(libnamehc,,,d)
}else: {
    LIBS += $$lib
}

OTHER_FILES += \        
	$$PWD/include/hardwareclient_global.h \
	$$PWD/include/hardwareclient.h \
    $$PWD/include/ioutgoingcommand.h \
    $$PWD/include/changenotifystatysincomingcommand.h \
    $$PWD/include/changehardwarestatusincomingcommand.h \
    $$PWD/include/iincomingcommand.h \
    $$PWD/include/startnotifyoutgoingcommand.h \
    $$PWD/include/requesthardwarestatusoutgoingcommand.h

libfileshc.path = $$DESTDIR
libfileshc.files = $$join(libname,,$$PWD/bin/,.*)
INSTALLS += libfileshc

include($$PWD/../AbstractAsyncClient/abstractasyncclient_lib.pri)

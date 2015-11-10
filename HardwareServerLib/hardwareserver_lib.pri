QMAKE_CXXFLAGS += \
    -I$$PWD/include/

LIBS += -L$$PWD/bin

libname = hardwareserver

greaterThan(QT_MAJOR_VERSION, 4): {
    lib = -lhardwareserver5
    libname = $$join(libname,,,5)
} else {
    lib = -lhardwareserver
}

CONFIG(debug, debug|release): {
    LIBS += $$join(lib,,,d)
    libname = $$join(libname,,,d)
}else: {
    LIBS += $$lib
}

OTHER_FILES += \
    $$PWD/include/iincomingcommand.h \
    $$PWD/include/hardwareserver.h\
    $$PWD/include/src_global.h \
    $$PWD/include/startnotifyincomingcommand.h \
    $$PWD/include/ioutgoingcommand.h \
    $$PWD/include/requesthardwarestatusincomingcommand.h \
    $$PWD/include/notifystatuschangeoutgoingcommand.h \
    $$PWD/include/hardwarestatuschangeoutgoingcommand.h \
    $$PWD/include/messageoutgoingcommand.h

libfileshs.path = $$DESTDIR
libfileshs.files = $$join(libname,,$$PWD/bin/,.*)
INSTALLS += libfileshs

include($$PWD/../AbstractAsyncClient/abstractasyncclient_lib.pri)

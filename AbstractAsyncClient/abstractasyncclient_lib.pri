QMAKE_CXXFLAGS += \
    -I$$PWD/include/

LIBS += -L$$PWD/bin

libname = abstractasyncclient
greaterThan(QT_MAJOR_VERSION, 4): {
    lib = -labstractasyncclient5
    libname = $$join(libname,,,5)
} else {
    lib = -labstractasyncclient
}

CONFIG(debug, debug|release): {
    LIBS += $$join(lib,,,d)
    libname = $$join(libname,,,d)
}else: {
    LIBS += $$lib
}

OTHER_FILES += \



# message($$libname)
libfilesaac.path = $$DESTDIR
libfilesaac.files = $$join(libname,,$$PWD/bin/,.*)
INSTALLS += libfilesaac


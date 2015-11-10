TEMPLATE = subdirs

SUBDIRS += \
    src \
    ..\AbstractAsyncClient \
    HardwareServerTestApp

src.depends = ..\AbstractAsyncClient
HardwareServerTestApp.depends = src

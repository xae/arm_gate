TEMPLATE = subdirs

SUBDIRS += \
    src \
    ../AbstractAsyncClient \
    HardwareClientTestApp

src.depends = ../AbstractAsyncClient
HardwareClientTestApp.depends = src

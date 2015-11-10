#-------------------------------------------------
#
# Project created by QtCreator 2015-10-21T13:47:08
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += \
    src \
    qhttpserver

src.depends += qhttpserver

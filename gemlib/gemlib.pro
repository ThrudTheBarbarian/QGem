QT +=  widgets network
QT -= gui

include($$PWD/../gemlib/gemlib.pri)

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    clientmsg.cc \
    socketthread.cc \
    vdi.cc \
    ops/v_opnvwk.cc \

HEADERS += \
    clientmsg.h \
    socketthread.h \
    vdi.h \
    constants.h \
    cvdi.h \
    gemdefs.h

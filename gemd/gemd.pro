QT += gui widgets network openglwidgets

CONFIG+=sdk_no_version_check

INCLUDEPATH += $$PWD/../shared

SOURCES += \
    clientmsg.cc \
    connectionmgr.cc \
    glwidget.cc \
    main.cc \
    screen.cc \
    socketthread.cc \
    vdi.cc

HEADERS += \
    clientmsg.h \
    connectionmgr.h \
    glwidget.h \
    screen.h \
    socketthread.h \
    vdi.h


FORMS += \
    screen.ui

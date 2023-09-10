QT += gui widgets network openglwidgets

CONFIG+=sdk_no_version_check

INCLUDEPATH +=  $$PWD/../shared

SOURCES += \
    clientmsg.cc \
    connectionmgr.cc \
    glwidget.cc \
    main.cc \
    ops/v_opnvwk.cc \
    screen.cc \
    socketthread.cc \
    vdi.cc \
    workstation.cc

HEADERS += \
    clientmsg.h \
    connectionmgr.h \
    glwidget.h \
    screen.h \
    socketthread.h \
    vdi.h \
    workstation.h


FORMS += \
    screen.ui

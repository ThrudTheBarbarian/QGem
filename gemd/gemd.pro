QT += gui widgets network openglwidgets

include($$PWD/../gemlib/gemlib.pri)


SOURCES += \
    connectionmgr.cc \
    glwidget.cc \
    main.cc \
    screen.cc

HEADERS += \
    connectionmgr.h \
    glwidget.h \
    main.h \
    screen.h


FORMS += \
    screen.ui

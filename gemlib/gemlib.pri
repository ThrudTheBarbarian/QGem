CONFIG+=sdk_no_version_check

INCLUDEPATH += $$PWD 
INCLUDEPATH += $$PWD/../shared

LIBS += -L$$PWD -L$$PWD/../../build/gemlib -lgemlib

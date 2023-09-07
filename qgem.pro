 TEMPLATE = subdirs

SUBDIRS += \
    gemd \
    gemlib \
    gemtest

gemd.depends = gemlib
gemtest.depends = gemlib


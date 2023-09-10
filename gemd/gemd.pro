QT += gui widgets network openglwidgets

CONFIG+=sdk_no_version_check

INCLUDEPATH +=  $$PWD/../shared

SOURCES += \
    clientmsg.cc \
    connectionmgr.cc \
    glwidget.cc \
    main.cc \
    ops/v_bit_image.cc \
    ops/v_clear_disp_list.cc \
    ops/v_clswk.cc \
    ops/v_curaddress.cc \
    ops/v_curdown.cc \
    ops/v_curhome.cc \
    ops/v_curleft.cc \
    ops/v_curright.cc \
    ops/v_curtext.cc \
    ops/v_curup.cc \
    ops/v_dspcur.cc \
    ops/v_eeol.cc \
    ops/v_eeos.cc \
    ops/v_enter_cur.cc \
    ops/v_form_adv.cc \
    ops/v_opnvwk.cc \
    ops/v_opnwk.cc \
    ops/v_output_window.cc \
    ops/v_rmcur.cc \
    ops/v_rvoff.cc \
    ops/v_rvon.cc \
    ops/v_updwk.cc \
    ops/vq_chcells.cc \
    ops/vq_curaddress.cc \
    ops/vq_exit_cur.cc \
    ops/vq_hardcopy.cc \
    ops/vq_tabstatus.cc \
    ops/vswr_mode.cc \
    screen.cc \
    socketthread.cc \
    v_clrwk.cc \
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

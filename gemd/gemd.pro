QT += gui widgets network openglwidgets

CONFIG+=sdk_no_version_check

INCLUDEPATH +=  $$PWD/../shared

SOURCES += \
    clientmsg.cc \
    connectionmgr.cc \
    fillfactory.cc \
    fontmgr.cc \
    glwidget.cc \
    main.cc \
    ops/v_bar.cc \
    ops/v_bit_image.cc \
    ops/v_circle.cc \
    ops/v_clear_disp_list.cc \
    ops/v_clrwk.cc \
    ops/v_clswk.cc \
    ops/v_curdown.cc \
    ops/v_curhome.cc \
    ops/v_curleft.cc \
    ops/v_curright.cc \
    ops/v_curtext.cc \
    ops/v_curup.cc \
    ops/v_dspcur.cc \
    ops/v_eeol.cc \
    ops/v_eeos.cc \
    ops/v_ellipse.cc \
    ops/v_ellpie.cc \
    ops/v_enter_cur.cc \
    ops/v_fillarea.cc \
    ops/v_form_adv.cc \
    ops/v_gtext.cc \
    ops/v_opnvwk.cc \
    ops/v_opnwk.cc \
    ops/v_output_window.cc \
    ops/v_pie.cc \
    ops/v_pline.cc \
    ops/v_pmarker.cc \
    ops/v_rfbox.cc \
    ops/v_rmcur.cc \
    ops/v_rvoff.cc \
    ops/v_rvon.cc \
    ops/v_updwk.cc \
    ops/vq_chcells.cc \
    ops/vq_curaddress.cc \
    ops/vq_exit_cur.cc \
    ops/vq_hardcopy.cc \
    ops/vq_tabstatus.cc \
    ops/vqt_name.cc \
    ops/vs_clip.cc \
    ops/vs_curaddress.cc \
    ops/vsf_color.cc \
    ops/vsf_interior.cc \
    ops/vsf_perimeter.cc \
    ops/vsf_style.cc \
    ops/vsl_color.cc \
    ops/vsl_ends.cc \
    ops/vsl_type.cc \
    ops/vsl_width.cc \
    ops/vsm_color.cc \
    ops/vsm_height.cc \
    ops/vsm_type.cc \
    ops/vst_alignment.cc \
    ops/vst_color.cc \
    ops/vst_effects.cc \
    ops/vst_font.cc \
    ops/vst_height.cc \
    ops/vst_load_fonts.cc \
    ops/vst_point.cc \
    ops/vst_rotation.cc \
    ops/vswr_mode.cc \
    screen.cc \
    socketthread.cc \
    vdi.cc \
    workstation.cc

HEADERS += \
    clientmsg.h \
    connectionmgr.h \
    fillfactory.h \
    fontmgr.h \
    glwidget.h \
    screen.h \
    socketthread.h \
    vdi.h \
    workstation.h


FORMS += \
    screen.ui

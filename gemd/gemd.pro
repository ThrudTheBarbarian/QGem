QT += gui widgets network openglwidgets

CONFIG+=sdk_no_version_check

INCLUDEPATH +=  $$PWD/../shared \
				$$PWD/io \
				$$PWD/system  \
				$$PWD/vdi

SOURCES += \
	aes/aes.cc \
    aes/appl_init.cc \
	io/clientmsg.cc \
    io/connectionmgr.cc \
    system/fillfactory.cc \
    system/fontmgr.cc \
    system/glwidget.cc \
    system/keymapper.cc \
    main.cc \
    vdi/setEventFilter.cc \
    vdi/v_arc.cc \
    vdi/v_bar.cc \
    vdi/v_bit_image.cc \
    vdi/v_circle.cc \
    vdi/v_clear_disp_list.cc \
    vdi/v_clrwk.cc \
    vdi/v_clswk.cc \
    vdi/v_contourfill.cc \
    vdi/v_curdown.cc \
    vdi/v_curhome.cc \
    vdi/v_curleft.cc \
    vdi/v_curright.cc \
    vdi/v_curtext.cc \
    vdi/v_curup.cc \
    vdi/v_dspcur.cc \
    vdi/v_eeol.cc \
    vdi/v_eeos.cc \
    vdi/v_ellarc.cc \
    vdi/v_ellipse.cc \
    vdi/v_ellpie.cc \
    vdi/v_enter_cur.cc \
    vdi/v_fillarea.cc \
    vdi/v_form_adv.cc \
    vdi/v_get_pixel.cc \
    vdi/v_gtext.cc \
    vdi/v_hide_c.cc \
    vdi/v_justified.cc \
    vdi/v_opnvwk.cc \
    vdi/v_opnwk.cc \
    vdi/v_output_window.cc \
    vdi/v_pie.cc \
    vdi/v_pline.cc \
    vdi/v_pmarker.cc \
    vdi/v_rbox.cc \
    vdi/v_rfbox.cc \
    vdi/v_rmcur.cc \
    vdi/v_rvoff.cc \
    vdi/v_rvon.cc \
    vdi/v_show_c.cc \
    vdi/v_updwk.cc \
    vdi/vex_timv.cc \
    vdi/vq_chcells.cc \
    vdi/vq_color.cc \
    vdi/vq_colours.cc \
    vdi/vq_curaddress.cc \
    vdi/vq_exit_cur.cc \
    vdi/vq_extnd.cc \
    vdi/vq_hardcopy.cc \
    vdi/vq_pixrgb.cc \
    vdi/vq_tabstatus.cc \
    vdi/vqf_attributes.cc \
    vdi/vqin_mode.cc \
    vdi/vql_attributes.cc \
    vdi/vqm_attributes.cc \
    vdi/vqt_attributes.cc \
    vdi/vqt_extent.cc \
    vdi/vqt_name.cc \
    vdi/vqt_width.cc \
    vdi/vro_cpyfm.cc \
    vdi/vrt_cpyfm.cc \
    vdi/vs_clip.cc \
    vdi/vs_color.cc \
    vdi/vs_curaddress.cc \
    vdi/vs_pixrgb.cc \
    vdi/vsc_form.cc \
    vdi/vsf_color.cc \
    vdi/vsf_interior.cc \
    vdi/vsf_perimeter.cc \
    vdi/vsf_style.cc \
    vdi/vsf_udpat.cc \
    vdi/vsf_udsty.cc \
    vdi/vsin_mode.cc \
    vdi/vsl_color.cc \
    vdi/vsl_ends.cc \
    vdi/vsl_type.cc \
    vdi/vsl_width.cc \
    vdi/vsm_color.cc \
    vdi/vsm_height.cc \
    vdi/vsm_type.cc \
    vdi/vst_alignment.cc \
    vdi/vst_color.cc \
    vdi/vst_effects.cc \
    vdi/vst_font.cc \
    vdi/vst_height.cc \
    vdi/vst_load_fonts.cc \
    vdi/vst_point.cc \
    vdi/vst_rotation.cc \
    vdi/vswr_mode.cc \
    system/screen.cc \
    io/socketthread.cc \
    vdi/vdi.cc \
    vdi/workstation.cc

HEADERS += \
    aes/aes.h \
    io/clientmsg.h \
    io/connectionmgr.h \
    system/fillfactory.h \
    system/fontmgr.h \
    system/glwidget.h \
    system/keymapper.h \
    system/screen.h \
    io/socketthread.h \
    vdi/vdi.h \
    vdi/workstation.h


FORMS += \
    screen.ui

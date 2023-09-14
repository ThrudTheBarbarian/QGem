//
//  vdi.h
//  client
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef vdi_h
#define vdi_h


/*****************************************************************************\
|*   3   : Clear a physical workstation (and all virtual workstations attached)
\*****************************************************************************/
void v_clrwk(void);

/*****************************************************************************\
|*   5.1 : Get the number of character cells on the alpha screen
\*****************************************************************************/
void vq_chcells(int16_t handle, int16_t* rows, int16_t* cols);

/*****************************************************************************\
|*   5.2 : Exit alpha mode
\*****************************************************************************/
void vq_exit_cur(int16_t handle);

/*****************************************************************************\
|*   5.3 : Enter alpha mode
\*****************************************************************************/
void v_enter_cur(int16_t handle);

/*****************************************************************************\
|*   5.4 : Cursor up if possible
\*****************************************************************************/
void v_curup(int16_t handle);

/*****************************************************************************\
|*   5.5 : Cursor down if possible
\*****************************************************************************/
void v_curdown(int16_t handle);

/*****************************************************************************\
|*   5.6 : Cursor right if possible
\*****************************************************************************/
void v_curright(int16_t handle);

/*****************************************************************************\
|*   5.7 : Cursor left if possible
\*****************************************************************************/
void v_curleft(int16_t handle);

/*****************************************************************************\
|*   5.8 : Cursor home (to 0,0)
\*****************************************************************************/
void v_curhome(int16_t handle);

/*****************************************************************************\
|*   5.9 : Erase to end of screen
\*****************************************************************************/
void v_eeos(int16_t handle);

/*****************************************************************************\
|*   5.10 : Erase to end of line
\*****************************************************************************/
void v_eeol(int16_t handle);

/*****************************************************************************\
|*   5.11 : Get the number of character cells on the alpha screen
\*****************************************************************************/
void vs_curadress(int16_t handle, int16_t row, int16_t col);

/*****************************************************************************\
|*   5.12 : Output text to the current cursor position
\*****************************************************************************/
void v_curtext(int16_t handle, const char *str);

/*****************************************************************************\
|*   5.13 : Enable reverse-video
\*****************************************************************************/
void v_rvon(int16_t handle);

/*****************************************************************************\
|*   5.14 : Disable reverse-video
\*****************************************************************************/
void v_rvoff(int16_t handle);

/*****************************************************************************\
|*   5.15 : Get the position of the cursor on the screen
\*****************************************************************************/
void vq_curaddress(int16_t handle, int16_t* row, int16_t* col);

/*****************************************************************************\
|*   5.16 : Enquire tablet status. Not implemented
\*****************************************************************************/
int vq_tabstatus(void);

/*****************************************************************************\
|*   5.17 : Copy the screen to the printer. Not implemented
\*****************************************************************************/
void vq_hardcopy(void);

/*****************************************************************************\
|*   5.18 : Display the mouse at position (x,y)
\*****************************************************************************/
void v_dspcur(int16_t handle, int16_t x, int16_t y);

/*****************************************************************************\
|*   5.19 : Hide the mouse cursor
\*****************************************************************************/
void v_rmcur(int16_t handle);

/*****************************************************************************\
|*   5.20 : Do a printer form-feed. Not implemented
\*****************************************************************************/
void v_form_adv(int16_t handle);

/*****************************************************************************\
|*   5.21 : Print an area of the screen. Not implemented
\*****************************************************************************/
void v_output_window(int16_t handle, int16_t *pts);

/*****************************************************************************\
|*   5.22 : Clear the printer display-list. Not implemented
\*****************************************************************************/
void v_clear_disp_list(int16_t handle);

/*****************************************************************************\
|*   5.23 : Print a bit-image file. Not implemented
\*****************************************************************************/
void v_bit_image(int16_t handle, const char *filename, int16_t aspect,
				 int16_t scaling, int16_t numPts, int16_t *pts);
	
/*****************************************************************************\
|*    6  : Draw a poly-line, with at least one point
\*****************************************************************************/
void v_pline(int16_t handle, int16_t numPts, int16_t*pts);

/*****************************************************************************\
|*    7  : Draw a poly-marker, with at least one point
\*****************************************************************************/
void v_pmarker(int16_t handle, int16_t numPts, int16_t*pts);

/*****************************************************************************\
|*   15  : Set the line-type (ie: dash-pattern)
\*****************************************************************************/
void vsl_type(int16_t handle, int16_t which);

/*****************************************************************************\
|*   16  : Set the line-width
\*****************************************************************************/
void vsl_width(int16_t handle, int16_t width);

/*****************************************************************************\
|*   17  : Set the line-colour
\*****************************************************************************/
void vsl_color(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   18  : Set the marker-type
\*****************************************************************************/
void vsm_type(int16_t handle, int16_t which);

/*****************************************************************************\
|*   19  : Set the marker-height
\*****************************************************************************/
void vsm_height(int16_t handle, int16_t height);

/*****************************************************************************\
|*   20  : Set the marker-colour
\*****************************************************************************/
void vsm_color(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   22  : Set the text-colour
\*****************************************************************************/
void vst_color(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   39  : Set the text-alignment
\*****************************************************************************/
void vst_alignment(int16_t handle, int16_t hIn,   int16_t vIn,
								   int16_t *hOut, int16_t *vOut);

/*****************************************************************************\
|*  100  : Open a virtual workstation
\*****************************************************************************/
void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut);

/*****************************************************************************\
|*  108  : Set the end-caps for lines
\*****************************************************************************/
void vsl_ends(int16_t handle, int16_t start, int16_t end);

/*****************************************************************************\
|*   129  : Set the clip rectangle
\*****************************************************************************/
void vs_clip(int16_t handle, int16_t enable, int16_t* pxy);


#endif /* vdi_h */

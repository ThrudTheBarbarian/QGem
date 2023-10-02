//
//  vdi.h
//  client
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef vdi_h
#define vdi_h

#include "gem.h"

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
|*    8  : Draw graphic text
\*****************************************************************************/
void v_gtext(int16_t handle, int16_t x, int16_t y, char *str);

/*****************************************************************************\
|*    8  : Fill a polygon
\*****************************************************************************/
void v_fillarea(int16_t handle, int16_t numPts, int16_t* pxy);

/*****************************************************************************\
|*   11.1	Fill a rectangle		[type=1] [pxy=x0,y0,x1,y1
\*****************************************************************************/
void v_bar(int16_t handle, int16_t*pts);

/*****************************************************************************\
|*   11.2: Draw an arc
\*****************************************************************************/
void v_arc(int16_t handle, int16_t x, int16_t y, int16_t radius,
		   int16_t beginAngle, int16_t endAngle);

/*****************************************************************************\
|*   11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
\*****************************************************************************/
void v_pie(int16_t handle, int16_t x, int16_t y, int16_t radius,
		   int16_t beginAngle, int16_t endAngle);

/*****************************************************************************\
|*   11.4: Fill a circle			[type=4] [pxy=x,y,r]
\*****************************************************************************/
void v_circle(int16_t handle, int16_t x, int16_t y, int16_t radius);

/*****************************************************************************\
|*  11.5: Fill an ellipse			[type=5] [pxy=x,y,rx,ry]
\*****************************************************************************/
void v_ellipse(int16_t handle, int16_t x, int16_t y, int16_t rx, int16_t ry);

/*****************************************************************************\
|*  11.6: Draw an elliptical arc
\*****************************************************************************/
void v_ellarc(int16_t handle, int16_t x, int16_t y,
			  int16_t xr, int16_t yr,
		      int16_t beginAngle, int16_t endAngle);

/*****************************************************************************\
|*  11.7: Fill an elliptical pie	[type=7] [pxy=x,y,xr,yr,begin,end]
\*****************************************************************************/
void v_ellpie(int16_t handle, int16_t x, int16_t y, int16_t rx, int16_t ry,
			  int16_t beginAngle, int16_t endAngle);

/*****************************************************************************\
|*  11.8: Draw a rounded rect
\*****************************************************************************/
void v_rbox(int16_t handle, int16_t* pxy);

/*****************************************************************************\
|*  11.9: Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
\*****************************************************************************/
void v_rfbox(int16_t handle, int16_t*pts);

/*****************************************************************************\
|* 11.10: Draw justified text. Note that in this implementation, the hints for
|*        spacing are silently ignored
\*****************************************************************************/
void v_justified(int16_t handle, int16_t x, int16_t y, int16_t length,
				 int16_t wordSpacing, int16_t charSpacing,
				 const char *str);

/*****************************************************************************\
|*   12  : Set the font-height in pixels
\*****************************************************************************/
void vst_height(int16_t handle, int16_t height,
				int16_t* charWidth, int16_t* charHeight,
				int16_t* cellWidth, int16_t* cellHeight);

/*****************************************************************************\
|*   13  : Set the text rotation in 1/10 of a degree
\*****************************************************************************/
int vst_rotation(int16_t handle, int16_t angle);

/*****************************************************************************\
|*   14  : Set the colour of a pen (0..255)
\*****************************************************************************/
void vs_color(int16_t handle, int16_t idx, int16_t* rgb);

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
|*   21  : Set the font
\*****************************************************************************/
int vst_font(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   22  : Set the text-colour
\*****************************************************************************/
void vst_color(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   23  : Set the fill-pattern type
\*****************************************************************************/
int vsf_interior(int16_t handle, int16_t which);

/*****************************************************************************\
|*   24  : Set the fill-pattern index
\*****************************************************************************/
int vsf_style(int16_t handle, int16_t which);

/*****************************************************************************\
|*   25  : Set the fill-pattern colour
\*****************************************************************************/
int vsf_color(int16_t handle, int16_t idx);

/*****************************************************************************\
|*   26 : Get the RGB values of a colour index
\*****************************************************************************/
void vq_color(int16_t handle, int16_t idx, int16_t flag, int16_t* rgb);

/*****************************************************************************\
|*   28 : Request the value of the input locator (mouse position)
\*****************************************************************************/
void vrq_locator(int16_t handle, int16_t x, int16_t y,
				 int16_t* x1, int16_t* y1, int16_t *term);

/*****************************************************************************\
|*   28 : Sample the value of the input locator (mouse position)
\*****************************************************************************/
void vsm_locator(int16_t handle, int16_t x, int16_t y,
				 int16_t* x1, int16_t* y1, int16_t *term);

/*****************************************************************************\
|*   31 : Request a string input (ie: read the keyboard until return pressed)
|*
|* I can't see any difference between vrq_string and vsm_string from the docs,
|* so just #define'ing it to be the same for now
\*****************************************************************************/
void vrq_string(int16_t handle, int16_t maxLen, int16_t echoMode,
				 int16_t* xy, char* str);
#define vsm_string vrq_string

/*****************************************************************************\
|*   32  : Set the writing-mode
\*****************************************************************************/
void vswr_mode(int16_t handle, int16_t which);

/*****************************************************************************\
|*   33  : Set the input-mode
\*****************************************************************************/
void vsin_mode(int16_t handle, int16_t device, int16_t mode);

/*****************************************************************************\
|*   35  : Get the current line attributes
|*         returns: type, pen, mode, width
\*****************************************************************************/
void vql_attributes(int16_t handle, int16_t*settings);

/*****************************************************************************\
|*   36  : Get the current marker attributes
|*         returns: type, pen, mode, height (pix)
\*****************************************************************************/
void vqm_attributes(int16_t handle, int16_t*settings);

/*****************************************************************************\
|*   37  : Get the current fill attributes
|*         returns: interior style, colour, pattern index, perimeter
\*****************************************************************************/
void vqf_attributes(int16_t handle, int16_t*settings);

/*****************************************************************************\
|*   38  : Get the current text attributes
|*         returns: font, colour, angle, halign, valign, mode, charW, charH,
|*                  cellW, cellH
\*****************************************************************************/
void vqt_attributes(int16_t handle, int16_t*settings);

/*****************************************************************************\
|*   39  : Set the text-alignment
\*****************************************************************************/
void vst_alignment(int16_t handle, int16_t hIn,   int16_t vIn,
								   int16_t *hOut, int16_t *vOut);

/*****************************************************************************\
|*   59  : Set a pixel to a colour
\*****************************************************************************/
void vs_pixrgb(int16_t handle, int16_t x,   int16_t y, int16_t *rgb);

/*****************************************************************************\
|*   60  : Get a pixel's colour
\*****************************************************************************/
void vq_pixrgb(int16_t handle, int16_t x,   int16_t y, int16_t *rgb);

/*****************************************************************************\
|*  100  : Open a virtual workstation
\*****************************************************************************/
void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut);

/*****************************************************************************\
|*  101  : Close a virtual workstation
\*****************************************************************************/
void v_clsvwk(int handle);

/*****************************************************************************\
|*  102  : Enquire about workstation details
\*****************************************************************************/
void vq_extnd(int16_t handle, int16_t extend, int16_t *workOut);

/*****************************************************************************\
|*  103  : Perform a flood-fill
\*****************************************************************************/
void v_contourfill(int16_t handle, int16_t x, int16_t y, int16_t index);

/*****************************************************************************\
|*  104  : Set whether to draw a perimeter around a fill
\*****************************************************************************/
int vsf_perimeter(int16_t handle, int16_t enable);

/*****************************************************************************\
|*  105  : Get a pixel. This is compatible with the old GEM interface
|*        	but only works to 16-bit colour resolution. Use vq_pixrgb
|*			(opcode 60) to get 24-bit RGB
\*****************************************************************************/
void v_get_pixel(int16_t handle, int16_t x, int16_t y,
				 int16_t* pixel, int16_t *idx);

/*****************************************************************************\
|*  106  : Set the text-effects
\*****************************************************************************/
int vst_effects(int16_t handle, int16_t effect);

/*****************************************************************************\
|*  107  : Set the font-height in points
\*****************************************************************************/
void vst_point(int16_t handle, int16_t height,
			   int16_t* charWidth, int16_t* charHeight,
			   int16_t* cellWidth, int16_t* cellHeight);

/*****************************************************************************\
|*  108  : Set the end-caps for lines
\*****************************************************************************/
void vsl_ends(int16_t handle, int16_t start, int16_t end);

/*****************************************************************************\
|*  109  : Blit opaque
\*****************************************************************************/
void vro_cpyfm(int16_t handle, int16_t mode, int16_t *pxy,
			   MFDB *src, MFDB *dst);

/*****************************************************************************\
|*  110  : Convert planar to chunky and back
\*****************************************************************************/
void vr_trnfm(int16_t handle, MFDB *src, MFDB *dst);

/*****************************************************************************\
|*  111  : Set the mouse form
|*
|* pform[0]	X-coordinate, and
|* pform[1]	Y-coordinate of hot-spot
|* pform[2]	Number of planes, must be 1
|* pform[3]	Colour index of background mask (should be 0)
|* pform[4]	Colour index of foreground mask (should be 1)
|* pform[05..20]	Definition of background mask
|* pform[21..36]	Definition of foreground mask
\*****************************************************************************/
void vsc_form(int16_t handle, int16_t *pform);

/*****************************************************************************\
|*  119  : Load in any extra fonts
\*****************************************************************************/
int vst_load_fonts(int16_t handle, int16_t dummy);

/*****************************************************************************\
|*   129  : Set the clip rectangle
\*****************************************************************************/
void vs_clip(int16_t handle, int16_t enable, int16_t* pxy);

/*****************************************************************************\
|*   130  : Get the name of a font on the server. Name must point to at least
|*          a 32-char buffer
\*****************************************************************************/
int16_t vqt_name(int16_t handle, int16_t fontId, char *name);

/*****************************************************************************\
|*   1090  : Query a palette in effect right now - either the 16-colour palette
|*           or the 256-colour palette
\*****************************************************************************/
void vq_colours(int16_t handle, int16_t cnum, int16_t *rgb);


#endif /* vdi_h */

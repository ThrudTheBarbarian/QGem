#ifndef VDI_H
#define VDI_H

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QObject>

#include "gem.h"
#include "properties.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class ClientMsg;
class SocketThread;
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class VDI : public QObject
	{
	Q_OBJECT

	public:
		/*********************************************************************\
		|* Typedefs and enums
		\*********************************************************************/
		typedef enum
			{
			POLY			= 0,
			RECT			= 1,
			PIE				= 3,
			CIRCLE			= 4,
			ELLIPSE			= 5,
			ELLIPTICAL_PIE	= 7,
			ROUNDED_RECT	= 9
			} FillType;

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GETSET(std::string, rootDir, RootDir);		// "System" disk root-dir
		GET(int, charWidth);						// Width of a sys-font char
		GET(int, charHeight);						// Height of same char
		GET(bool, alphaMode);						// Cursors or graphics
		GET(int, cursorX);							// Current cursor X
		GET(int, cursorY);							// Current cursor Y
		GETSET(bool, reverseVideo, ReverseVideo);	// Draw in reverse video ?
		GETSETP(Workstation*, top, Top);			// Workstation with focus
		GETSET(QRect, mouseArea1, MouseArea1);		// Notify if enter/exit

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		SocketThread *_io;				// Thread to run socket io
		Workstation *_dpy;				// Physical workstation
		Screen *_screen;				// Main window
		QImage *_img;					// Where drawing happens
		uint64_t _frames;				// Frame counter
		bool _cursorShown;				// Cursor is actually drawn
		QImage _cursorBacking;			// What was underneath the cursor

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit VDI(QObject *parent = nullptr);

	public:
		/*********************************************************************\
		|* Get a reference to the VDI
		\*********************************************************************/
		static VDI& sharedInstance(void)
			{
			static VDI _instance;               // The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* Delete methods that might create an instance involuntarily
		\*********************************************************************/
		VDI(VDI const&)                 = delete;
		void operator=(VDI const&)      = delete;

		/*********************************************************************\
		|* Get/Set the main window (screen)
		\*********************************************************************/
		inline void setScreen(Screen *s)
			{ _screen = s; }
		inline Screen * screen(void)
			{ return _screen; }


		/*********************************************************************\
		|* Get the backing image that everything draws into
		\*********************************************************************/
		inline QImage * bg(void)
			{ return _img; }


		#pragma mark - VDI operations

		/*********************************************************************\
		|*   1: open a physical workstation
		\*********************************************************************/
		void v_opnwk(int16_t *workIn, int16_t *handle, int16_t *workOut);

		/*********************************************************************\
		|*   2: close a physical workstation
		\*********************************************************************/
		void v_clswk(qintptr handle);

		/*********************************************************************\
		|*   3: Clear a physical workstation
		\*********************************************************************/
		void v_clrwk(Workstation *ws);

		/*********************************************************************\
		|*   4: Update a physical workstation
		\*********************************************************************/
		void v_updwk(qintptr handle);

		/*********************************************************************\
		|*   5.1: Query the number of character cells on the alpha screen
		\*********************************************************************/
		void vq_chcells(qintptr handle, int16_t& rows, int16_t& columns);
		void vq_chcells(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   5.2: Exit alpha mode
		\*********************************************************************/
		void vq_exit_cur(Workstation *ws);

		/*********************************************************************\
		|*   5.3: Enter alpha mode
		\*********************************************************************/
		void v_enter_cur(Workstation *ws);

		/*********************************************************************\
		|*   5.4: Move the cursor up if possible
		\*********************************************************************/
		void v_curup(qintptr handle);
		void v_curup(Workstation *ws);

		/*********************************************************************\
		|*   5.5: Move the cursor down if possible
		\*********************************************************************/
		void v_curdown(qintptr handle);
		void v_curdown(Workstation *ws);

		/*********************************************************************\
		|*   5.6: Move the cursor right if possible
		\*********************************************************************/
		void v_curright(qintptr handle);
		void v_curright(Workstation *ws);

		/*********************************************************************\
		|*   5.7: Move the cursor right if possible
		\*********************************************************************/
		void v_curleft(qintptr handle);
		void v_curleft(Workstation *ws);

		/*********************************************************************\
		|*   5.8: Move the cursor home (0,0)
		\*********************************************************************/
		void v_curhome(qintptr handle);
		void v_curhome(Workstation *ws);

		/*********************************************************************\
		|*   5.9: Erase to end of screen
		\*********************************************************************/
		void v_eeos(Workstation *ws);

		/*********************************************************************\
		|*   5.10: Erase to end of line
		\*********************************************************************/
		void v_eeol(Workstation *ws);

		/*********************************************************************\
		|*   5.11: Move cursor
		\*********************************************************************/
		void vs_curaddress(qintptr handle, int16_t row, int16_t col);
		void vs_curaddress(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   5.12: Draw text at the cursor position
		\*********************************************************************/
		void v_curtext(Workstation *ws, const char* str);
		void v_curtext(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   5.13: Enable reverse-video
		\*********************************************************************/
		void v_rvon(qintptr handle);
		void v_rvon(Workstation *ws);

		/*********************************************************************\
		|*   5.14: Disable reverse-video
		\*********************************************************************/
		void v_rvoff(qintptr handle);
		void v_rvoff(Workstation *ws);

		/*********************************************************************\
		|*   5.15: Get the current cursor position
		\*********************************************************************/
		void vq_curaddress(qintptr handle, int16_t& row, int16_t& col);
		void vq_curaddress(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   5.16: Get the tablet status
		\*********************************************************************/
		int vq_tabstatus(qintptr handle);

		/*********************************************************************\
		|*   5.17: Dump the screen to an attached printer
		\*********************************************************************/
		void vq_hardcopy(qintptr handle);

		/*********************************************************************\
		|*   5.18: Position the graphical cursor (mouse)
		\*********************************************************************/
		void v_dspcur(qintptr handle, int16_t x, int16_t y);
		void v_dspcur(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   5.19: Hide the graphical cursor (mouse)
		\*********************************************************************/
		void v_rmcur(qintptr handle);
		void v_rmcur(Workstation *ws);

		/*********************************************************************\
		|*   5.20: Form-feed. Only useful on printers, not supported
		\*********************************************************************/
		void v_form_adv(qintptr handle);

		/*********************************************************************\
		|*   5.21: Output window to printer, not supported
		\*********************************************************************/
		void v_output_window(qintptr handle);

		/*********************************************************************\
		|*   5.22: Clear printer display-list, not supported
		\*********************************************************************/
		void v_clear_disp_list(qintptr handle);

		/*********************************************************************\
		|*   5.23: Print an image, not supported
		\*********************************************************************/
		void v_bit_image(qintptr handle, const char *filename,
						 int16_t aspect, int16_t scaling, int16_t numPts,
						 int16_t *pts);

		/*********************************************************************\
		|*   6: Draw a poly-line
		\*********************************************************************/
		void v_pline(qintptr handle, int16_t numPts, int16_t *pxy);
		void v_pline(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   7: Draw a poly-marker
		\*********************************************************************/
		void v_pmarker(qintptr handle, int16_t numPts, int16_t *pxy);
		void v_pmarker(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   8: Draw graphic text
		\*********************************************************************/
		void v_gtext(qintptr handle, int16_t x, int16_t y, int16_t w, char *s);
		void v_gtext(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*   9	: Fill a polygon			[type=0] ppxy=...]
		\*********************************************************************/
		void v_fillarea(qintptr handle, FillType type,
						int16_t numItems, int16_t *pxy);
		void v_fillarea(Workstation *ws, ClientMsg *cm);


		/*********************************************************************\
		|*  11.1: Fill a rectangle			[type=1] [pxy=x0,y0,x1,y1]
		\*********************************************************************/
		void v_bar(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.2: Draw an arc
		\*********************************************************************/
		void v_arc(qintptr handle, int16_t x, int16_t y, int16_t radius,
				   int16_t begang, int16_t endang);
		void v_arc(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
		\*********************************************************************/
		void v_pie(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.4: Fill a circle				[type=4] [pxy=x,y,r]
		\*********************************************************************/
		void v_circle(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.5: Fill an ellipse			[type=5] [pxy=x,y,rx,ry]
		\*********************************************************************/
		void v_ellipse(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.6: Draw an elliptical arc
		\*********************************************************************/
		void v_ellarc(qintptr handle, int16_t x, int16_t y,
					  int16_t rx, int16_t ry,
					  int16_t begang, int16_t endang);
		void v_ellarc(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.7: Fill an elliptical pie	[type=7] [pxy=x,y,xr,yr,begin,end]
		\*********************************************************************/
		void v_ellpie(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.8: Draw a rounded box
		\*********************************************************************/
		void v_rbox(qintptr handle, int16_t *pxy);
		void v_rbox(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  11.9: Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
		\*********************************************************************/
		void v_rfbox(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 11.10: Draw justfied text
		\*********************************************************************/
		void v_justified(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  12 Set the height of text drawn and get metrics
		\*********************************************************************/
		void vst_height(qintptr handle, int16_t height, int16_t& charWidth,
						int16_t& charHeight, int16_t& cellWidth,
						int16_t &cellHeight);
		void vst_height(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  13: Set the text-rotation angle
		\*********************************************************************/
		void vst_rotation(qintptr handle, int16_t angle);
		void vst_rotation(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  13: Set the text-rotation angle
		\*********************************************************************/
		void vs_color(qintptr handle, int16_t which, int16_t* rgb);
		void vs_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  15: Set the style (dash pattern) for drawing lines
		\*********************************************************************/
		void vsl_type(qintptr handle, int16_t idx);
		void vsl_type(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  16: Set the width for drawing lines
		\*********************************************************************/
		void vsl_width(qintptr handle, int16_t width);
		void vsl_width(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  17: Set the colour index for drawing lines
		\*********************************************************************/
		void vsl_color(qintptr handle, int16_t idx);
		void vsl_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  18: Set the type of marker drawn
		\*********************************************************************/
		void vsm_type(qintptr handle, int16_t idx);
		void vsm_type(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  19: Set the height of marker drawn
		\*********************************************************************/
		void vsm_height(qintptr handle, int16_t height);
		void vsm_height(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  20: Request the use of an identified font
		\*********************************************************************/
		int vst_font(qintptr handle, int16_t fontId);
		void vst_font(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  20: Set the colour of marker drawn
		\*********************************************************************/
		void vsm_color(qintptr handle, int16_t height);
		void vsm_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  22: Set the colour of any text drawn
		\*********************************************************************/
		void vst_color(qintptr handle, int16_t height);
		void vst_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  23: Set the fill-pattern type to use
		\*********************************************************************/
		void vsf_interior(qintptr handle, int16_t which);
		void vsf_interior(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  24: Set the fill-pattern index to use
		\*********************************************************************/
		void vsf_style(qintptr handle, int16_t which);
		void vsf_style(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  25: Set the fill-pattern colour to use
		\*********************************************************************/
		void vsf_color(qintptr handle, int16_t idx);
		void vsf_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  26: Get the RGB values of a colour index
		\*********************************************************************/
		void vq_color(qintptr handle, int16_t idx, int16_t *rgb);
		void vq_color(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  32: Set the writing mode
		\*********************************************************************/
		void vswr_mode(Workstation *ws, int16_t mode);
		void vswr_mode(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  33: Set the input mode
		\*********************************************************************/
		void vsin_mode(qintptr handle, int16_t device, int16_t mode);
		void vsin_mode(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  35: Get the current line style
		\*********************************************************************/
		void vql_attributes(qintptr handle, int16_t* settings);
		void vql_attributes(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  36: Get the current marker style
		\*********************************************************************/
		void vqm_attributes(qintptr handle, int16_t* settings);
		void vqm_attributes(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  37: Get the current fill style
		\*********************************************************************/
		void vqf_attributes(qintptr handle, int16_t* settings);
		void vqf_attributes(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  38: Get the current text style
		\*********************************************************************/
		void vqt_attributes(qintptr handle, int16_t* settings);
		void vqt_attributes(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  39: Request the text alignment and get the actual set values
		\*********************************************************************/
		void vst_alignment(qintptr handle, int16_t  hIn,  int16_t  vIn,
										   int16_t& hOut, int16_t& vOut);
		void vst_alignment(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  59: Set a pixel's RGB
		\*********************************************************************/
		void vs_pixrgb(qintptr handle, int16_t x, int16_t y, int16_t *rgb);
		void vs_pixrgb(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|*  60: Get a pixel's RGB
		\*********************************************************************/
		void vq_pixrgb(qintptr handle, int16_t x, int16_t y, int16_t *rgb);
		void vq_pixrgb(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 100: Open a virtual workstation
		\*********************************************************************/
		void v_opnvwk(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 100: Get extended information
		\*********************************************************************/
		void vq_extnd(qintptr handle, int16_t  extnd, int16_t* data);
		void vq_extnd(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 103: Flood fill an area
		\*********************************************************************/
		void v_contourfill(qintptr handle, int16_t x, int16_t y, int16_t colour);
		void v_contourfill(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 104: Set whether to draw a perimeter around a fill
		\*********************************************************************/
		void vsf_perimeter(qintptr handle, int16_t  enable);
		void vsf_perimeter(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 105: Get the pixel value. Note only returns pixel values to 16-bit
		|*      resolution. Use v_get_pixelx to get 24-bit RGB
		\*********************************************************************/
		void v_get_pixel(qintptr handle, int16_t x, int16_t y,
						 int16_t& pixel, int16_t& idx);
		void v_get_pixel(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 106: Set the way in which a font is rendered
		\*********************************************************************/
		void vst_effects(qintptr handle, int16_t effect);
		void vst_effects(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|*  107 Set the height of text drawn (in points) and get metrics
		\*********************************************************************/
		void vst_point(qintptr handle, int16_t height, int16_t& charWidth,
					   int16_t& charHeight, int16_t& cellWidth,
					   int16_t &cellHeight);
		void vst_point(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 108: Set the ends of drawn lines
		\*********************************************************************/
		void vsl_ends(qintptr handle, int16_t begin, int16_t end);
		void vsl_ends(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 109: Blit
		\*********************************************************************/
		void vro_cpyfm(qintptr handle, int16_t mode, int16_t *pxy,
					   MFDB *src, MFDB *dst);
		void vro_cpyfm(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 119: Load up the system fonts
		\*********************************************************************/
		int16_t vst_load_fonts(void);
		void vst_load_fonts(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 129: Set the clipping rectangle
		\*********************************************************************/
		void vs_clip(qintptr handle, int16_t enableClip, int16_t *pxy);
		void vs_clip(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 130: Get the name of a font by its index
		\*********************************************************************/
		int16_t vqt_name(qintptr handle, int16_t idx, char *name);
		void vqt_name(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 2026 : Get the colour values for a palette
		\*********************************************************************/
		void vq_colours(qintptr handle, int16_t*rgb);
		void vq_colours(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 17100 : Change the event filter value
		\*********************************************************************/
		void setEventFilter(Workstation *ws, ClientMsg *msg);

	public slots:
		/*********************************************************************\
		|* A frame has been drawn
		\*********************************************************************/
		void frameRendered(void);

	private:
		/*********************************************************************\
		|* Cursor drawing/erasing
		\*********************************************************************/
		bool _eraseCursor(void);
		void _drawCursor(void);

	};

#endif // VDI_H

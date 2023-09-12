#ifndef VDI_H
#define VDI_H

#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QObject>

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
		|* Properties
		\*********************************************************************/
		GETSET(QFont, systemFont, SystemFont);		// Main system font
		GETSET(std::string, rootDir, RootDir);		// "System" disk root-dir
		GET(int, charWidth);						// Width of a sys-font char
		GET(int, charHeight);						// Height of same char
		GET(bool, alphaMode);						// Cursors or graphics
		GET(int, cursorX);							// Current cursor X
		GET(int, cursorY);							// Current cursor Y
		GETSET(bool, reverseVideo, ReverseVideo);	// Draw in reverse video ?

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
		QFontMetrics *_fm;				// Font metrics for the system font

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
		|*   5.20: Form-feed. Only useful on printers, which aren't supported
		\*********************************************************************/
		void v_form_adv(qintptr handle);

		/*********************************************************************\
		|*   5.21: Output window to printer, which isn't supported
		\*********************************************************************/
		void v_output_window(qintptr handle);

		/*********************************************************************\
		|*   5.22: Clear printer display-list, which isn't supported
		\*********************************************************************/
		void v_clear_disp_list(qintptr handle);

		/*********************************************************************\
		|*   5.23: Print an image, which isn't supported
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
		|*  32: Set the writing mode
		\*********************************************************************/
		void vswr_mode(Workstation *ws, int16_t mode);
		void vswr_mode(Workstation *ws, ClientMsg *cm);

		/*********************************************************************\
		|* 100: Open a virtual workstation
		\*********************************************************************/
		void v_opnvwk(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 108: Set the ends of drawn lines
		\*********************************************************************/
		void vsl_ends(qintptr handle, int16_t begin, int16_t end);
		void vsl_ends(Workstation *ws, ClientMsg *msg);

		/*********************************************************************\
		|* 129: Set the clipping rectangle
		\*********************************************************************/
		void vs_clip(qintptr handle, int16_t enableClip, int16_t *pxy);
		void vs_clip(Workstation *ws, ClientMsg *msg);

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

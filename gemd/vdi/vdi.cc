#include <QPainter>

#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
VDI::VDI(QObject *parent)
	:QObject{parent}
	,_charWidth(6)
	,_charHeight(14)
	,_alphaMode(false)
	,_cursorX(0)
	,_cursorY(0)
	,_reverseVideo(false)
	,_top(nullptr)
	,_mouseArea1(QRect(0,0,0,0))
	,_io(nullptr)
	,_dpy(nullptr)
	,_screen(nullptr)
	,_frames(0)
	,_cursorHideCount(0)
	{}


#pragma mark - Slots

/*****************************************************************************\
|* A frame has been rendered, update the counter
\*****************************************************************************/
void VDI::frameRendered(void)
	{
	_frames ++;

	/**************************************************************************\
	|* Update the cursor if we need to
	\**************************************************************************/
	if (_alphaMode && ((_frames %30) == 0))
		{
		if (_alphaCursorShown)
			_eraseAlphaCursor();
		else
			_drawAlphaCursor();
		}
	}



#pragma mark - Private methods

/*****************************************************************************\
|* Erase the cursor
\*****************************************************************************/
bool VDI::_eraseAlphaCursor(void)
	{
	bool hid = false;

	if (_alphaCursorShown == true)
		{
		_alphaCursorShown	= false;
		int x				= _cursorX * _charWidth;
		int y				= _cursorY * _charHeight;
		int descent			= _dpy->fm()->descent();

		QPoint pt = {x,y+descent};
		QPainter painter(_img);
		painter.drawImage(pt,_cursorBacking);
		hid = true;
		}

	return hid;
	}

/*****************************************************************************\
|* Draw the cursor
\*****************************************************************************/
void VDI::_drawAlphaCursor(void)
	{
	if (_alphaCursorShown == false)
		{
		_alphaCursorShown	= true;
		int x				= _cursorX * _charWidth;
		int y				= _cursorY * _charHeight;
		int w				= _charWidth/2;
		int h				= _charHeight;
		int descent			= _dpy->fm()->descent();

		QRect r				= {x,y+descent,w,h};
		_cursorBacking		= _img->copy(r);

		QPainter painter(_img);
		painter.fillRect(r, _dpy->colour(1));
		}
	}

/*****************************************************************************\
|* Show the mouse cursor
\*****************************************************************************/
void VDI::_showCursor(bool enable, int16_t reset)
	{
	if (reset == 0)
		_cursorHideCount = 0;
	else if (enable)
		_cursorHideCount --;
	else
		_cursorHideCount ++;

	if (_cursorHideCount < 0)
		_cursorHideCount = 0;

	fprintf(stderr, "Cursor Hide count: %d\n", _cursorHideCount);
	if (_cursorHideCount > 0)
		_screen->setCursor(Qt::BlankCursor);
	else
		_screen->setCursor(Qt::ArrowCursor);
	}

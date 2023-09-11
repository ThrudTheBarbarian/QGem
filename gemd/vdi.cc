#include <QPainter>

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
	,_io(nullptr)
	,_dpy(nullptr)
	,_screen(nullptr)
	,_frames(0)
	,_fm(nullptr)
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
		if (_cursorShown)
			_eraseCursor();
		else
			_drawCursor();
		}
	}



#pragma mark - Private methods

/*****************************************************************************\
|* Erase the cursor
\*****************************************************************************/
bool VDI::_eraseCursor(void)
	{
	bool hid = false;

	if (_cursorShown == true)
		{
		_cursorShown = false;
		int x			= _cursorX * _charWidth;
		int y			= _cursorY * _charHeight;
		int descent		= _fm->descent();

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
void VDI::_drawCursor(void)
	{
	if (_cursorShown == false)
		{
		_cursorShown = true;
		int x			= _cursorX * _charWidth;
		int y			= _cursorY * _charHeight;
		int w			= _charWidth/2;
		int h			= _charHeight;
		int descent		= _fm->descent();

		QRect r			= {x,y+descent,w,h};
		_cursorBacking	= _img->copy(r);

		QPainter painter(_img);
		painter.fillRect(r, _dpy->colour(1));
		}
	}

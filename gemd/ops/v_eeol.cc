#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.10: Erase to end-of-line.
|*
|* Original signature is: v_eeol(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeol(Workstation *ws)
	{
	bool erased = _eraseCursor();

	QRect r = {_cursorX * _charWidth,
			   _cursorY * _charHeight,
			   _screen->width() - _cursorX * _charWidth,
			   _charHeight};

	QPainter painter(_img);
	painter.fillRect(r, ws->colour(0));

	if (erased)
		_drawCursor();
	}

#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.10: Erase to end-of-line.
|*
|* Original signature is: v_eeol(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeol(int16_t handle)
	{
	if (handle == 0)
		{
		bool erased = _eraseCursor();

		QRect r = {_cursorX * _charWidth,
				   _cursorY * _charHeight,
				   _screen->width() - _cursorX * _charWidth,
				   _charHeight};

		QPainter painter(_img);
		painter.fillRect(r, _palette[0]);

		if (erased)
			_drawCursor();
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

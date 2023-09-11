#include <QPainter>

#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.4: Move the cursor up if possible.
|*
|* Original signature is: v_curtext(int16_t handle, char *str);
|*
\*****************************************************************************/
void VDI::v_curtext(Workstation *ws, const char* str)
	{
	bool erased = _eraseCursor();

	QColor fg	= ws->colour((_reverseVideo) ? 0 : 1);
	QColor bg	= ws->colour((_reverseVideo) ? 1 : 0);

	QFontMetrics fm(_systemFont);
	int descent = fm.descent();
	QRect r		= {	_cursorX * _charWidth,
					_cursorY * _charHeight - descent,
					0,
					_charHeight};

	int len = (int) strlen(str);
	r.setWidth(_charWidth * len);


	QPainter P(_img);
	P.fillRect(r,bg);
	P.setPen(fg);
	P.drawText(r.x(), r.y() + _charHeight, str);

	int maxX = _screen->width() / _charWidth;
	_cursorX += len;
	if (_cursorX > maxX)
		_cursorX = maxX;

	if (erased)
		_drawCursor();
	}

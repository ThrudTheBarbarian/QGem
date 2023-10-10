#include <QPainter>

#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.9: Erase to end-of-screen.
|*
|* Original signature is: v_eeos(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeos(Workstation *ws)
	{
	v_eeol(ws);

	int H = _screen->height();
	int W = _screen->width();
	int y = (_cursorY + 1) * _charHeight;

	QRect r = {0, y, W, H-y};
	QPainter painter(_img);
	painter.fillRect(r, ws->colour(0));
	}

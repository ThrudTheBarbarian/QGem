#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.9: Erase to end-of-screen.
|*
|* Original signature is: v_eeos(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_eeos(int16_t handle)
	{
	if (handle == 0)
		{
		v_eeol(handle);

		int H = _screen->height();
		int W = _screen->width();
		int y = (_cursorY + 1) * _charHeight;

		QRect r = {0, y, W, H-y};
		QPainter painter(_img);
		painter.fillRect(r, _palette[0]);
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

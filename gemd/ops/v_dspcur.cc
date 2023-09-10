#include <QCursor>

#include "debug.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.10: Erase to end-of-line.
|*
|* Original signature is: v_eeol(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_dspcur(int16_t handle, int16_t x, int16_t y)
	{
	if (handle == 0)
		{
		QPoint p = _screen->mapToGlobal(QPoint(x,y));
		fprintf(stderr, "Move to (%d,%d)\n", p.x(), p.y());
		QCursor::setPos(p);
		_screen->setCursor(Qt::ArrowCursor);
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

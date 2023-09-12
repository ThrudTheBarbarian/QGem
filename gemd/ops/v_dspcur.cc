#include <QCursor>

#include "clientmsg.h"
#include "debug.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.10: Erase to end-of-line.
|*
|* Original signature is: v_eeol(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_dspcur(qintptr handle, int16_t x, int16_t y)
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

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_dspcur(Workstation *, ClientMsg *cm)
	{
	const Payload &p = cm->payload();

	/**************************************************************************\
	|* Get the data out of the message
	\**************************************************************************/
	int16_t x = p[0];
	int16_t y = p[1];
	v_dspcur(0, x, y);
	}

#include <QCursor>

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
void VDI::v_rmcur(int16_t handle)
	{
	if (handle == 0)
		{
		_screen->setCursor(Qt::BlankCursor);
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rmcur(Workstation *)
	{
	v_rmcur((int16_t)0);
	}

#include <QPainter>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 123: Hide the cursor.
|*
|* Original signature is: v_hide_c(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_hide_c(qintptr)
	{
	_showCursor(false, 1);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_hide_c(Workstation *ws, ClientMsg *)
	{
	v_hide_c(ws->handle());
	}

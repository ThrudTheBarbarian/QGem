#include <QPainter>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 122: Show the cursor.
|*
|* Original signature is: v_show_c(int16_t handle, int16_t reset);
|*
\*****************************************************************************/
void VDI::v_show_c(qintptr, int16_t reset)
	{
	_showCursor(true, reset);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_show_c(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t reset		= ntohs(p[0]);

	v_show_c(ws->handle(), reset);
	}

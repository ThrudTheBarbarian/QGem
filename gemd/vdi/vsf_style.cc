#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 24: Set the fill-pattern index.
|*
|* Original signature is: vsf_style(int16_t handle, int16_t which));
|*
\*****************************************************************************/
void VDI::vsf_style(qintptr handle, int16_t which)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (which < 0)
		which = 0;
	if (which > 24)
		which = 24;
	if (ws != nullptr)
		ws->setFillStyle(which);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_style(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= ntohs(p[0]);

	vsf_style(ws->handle(), style);
	}

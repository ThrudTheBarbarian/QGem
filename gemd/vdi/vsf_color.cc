#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 25: Set the fill-pattern colour.
|*
|* Original signature is: vsf_color(int16_t handle, int16_t idx));
|*
\*****************************************************************************/
void VDI::vsf_color(qintptr handle, int16_t idx)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (idx < 0)
		idx = 0;
	if (idx > 255)
		idx = 255;
	if (ws != nullptr)
		ws->setFillColour(idx);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= ntohs(p[0]);

	vsf_color(ws->handle(), style);
	}

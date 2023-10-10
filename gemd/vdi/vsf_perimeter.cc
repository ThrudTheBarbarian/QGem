#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 104: Set whether to draw a perimeter around a fill.
|*
|* Original signature is: vsf_perimeter(int16_t handle, int16_t enable));
|*
\*****************************************************************************/
void VDI::vsf_perimeter(qintptr handle, int16_t enable)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		ws->setDrawPerimeter(enable != 0);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_perimeter(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t enable		= ntohs(p[0]);

	vsf_perimeter(ws->handle(), enable);
	}

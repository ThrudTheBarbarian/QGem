#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 23: Set the fill-pattern style.
|*
|* Original signature is: vsf_interior(int16_t handle, int16_t type));
|*
\*****************************************************************************/
void VDI::vsf_interior(qintptr handle, int16_t which)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (which < FIS_HOLLOW)
		which = FIS_HOLLOW;
	if (which > FIS_USER)
		which = FIS_USER;
	if (ws != nullptr)
		ws->setFillType(which);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsf_interior(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= ntohs(p[0]);

	vsf_interior(ws->handle(), style);
	}

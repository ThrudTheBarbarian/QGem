#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 18: Set the type of marker drawn.
|*
|* Original signature is: vsm_type(int16_t handle, int16_t which);
|*
\*****************************************************************************/
void VDI::vsm_type(qintptr handle, int16_t which)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((which < MRKR_DOT) || (which > MRKR_CIRCLE))
		which = MRKR_CIRCLE;
	if (ws != nullptr)
		ws->setMarkerType(which);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_type(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t which		= ntohs(p[0]);

	vsm_type(ws->handle(), which);
	}

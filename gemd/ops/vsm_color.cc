#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 20: Set the colour for drawing markers.
|*
|* Original signature is: vsm_color(int16_t handle, int16_t idx);
|*
\*****************************************************************************/
void VDI::vsm_color(qintptr handle, int16_t idx)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((idx > 0) && (idx < 256))
		if (ws != nullptr)
			ws->setMarkerColour(idx);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t idx			= ntohs(p[0]);

	vsm_color(ws->handle(), idx);
	}

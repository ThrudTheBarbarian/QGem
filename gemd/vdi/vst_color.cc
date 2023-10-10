#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 22: Set the colour for drawing text.
|*
|* Original signature is: vst_color(int16_t handle, int16_t idx);
|*
\*****************************************************************************/
void VDI::vst_color(qintptr handle, int16_t idx)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((idx > 0) && (idx < 256))
		if (ws != nullptr)
			ws->setTextColour(idx);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t idx			= ntohs(p[0]);

	vst_color(ws->handle(), idx);
	}

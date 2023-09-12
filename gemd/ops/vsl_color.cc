#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 17: Set the colour for drawing lines.
|*
|* Original signature is: vsl_color(int16_t handle, int16_t idx);
|*
\*****************************************************************************/
void VDI::vsl_color(qintptr handle, int16_t idx)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((idx > 0) && (idx < 256))
		if (ws != nullptr)
			ws->setLineColour(idx);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t idx			= p[0];

	vsl_color(ws->handle(), idx);
	}

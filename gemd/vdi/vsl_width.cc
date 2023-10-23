#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 16: Set the width for drawing lines.
|*
|* Original signature is: vsl_width(int16_t handle, int16_t width);
|*
\*****************************************************************************/
void VDI::vsl_width(qintptr handle, int16_t width)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (width < 1)
		width = 1;
	if (ws != nullptr)
		ws->setLineWidth(width);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_width(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= ntohs(p[0]);

	vsl_width(ws->handle(), style);
	}

#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 15: Set the style for drawing lines.
|*
|* Original signature is: vsl_color(int16_t handle, int16_t idx);
|*
\*****************************************************************************/
void VDI::vsl_type(qintptr handle, int16_t idx)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((idx < SOLID) || (idx > USERLINE))
		idx = SOLID;
	if (ws != nullptr)
		ws->setLineType(idx);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_type(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= p[0];

	vsl_type(ws->handle(), style);
	}

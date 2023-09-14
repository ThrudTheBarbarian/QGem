#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 13: Set the text-rotation angle.
|*
|* Original signature is: vst_rotation(int16_t handle, int16_t angle));
|*
\*****************************************************************************/
void VDI::vst_rotation(qintptr handle, int16_t angle)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (angle < 0)
		angle = 0;
	if (ws != nullptr)
		ws->setTextRotation((angle % 3600)/10);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_rotation(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= p[0];

	vst_rotation(ws->handle(), style);
	}

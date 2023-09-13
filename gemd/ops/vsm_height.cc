#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 19: Set the height of marker in pixels.
|*
|* Original signature is: vsm_height(int16_t handle, int16_t height);
|*
\*****************************************************************************/
void VDI::vsm_height(qintptr handle, int16_t height)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	height = (height < 1) ? 1
		   : (height > 255) ? 255
		   : ((height &1) == 0) ? height + 1
		   : height;

	if (ws != nullptr)
		ws->setMarkerHeight(height);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsm_height(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t height		= p[0];

	vsm_height(ws->handle(), height);
	}

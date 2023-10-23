#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 106: Set the effects used for drawing text.
|*
|* Original signature is: vst_effects(int16_t handle, int16_t effect);
|*
\*****************************************************************************/
void VDI::vst_effects(qintptr handle, int16_t effect)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		ws->setTextEffect(effect & TXT_MASK);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_effects(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t effect		= ntohs(p[0]);

	vst_effects(ws->handle(), effect);
	}

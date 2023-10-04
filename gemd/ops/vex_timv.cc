#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 118: Register an event-based timer callback.
|*
|* Original signature is: vex_timv(int16_t handle,
|*								   GemTimerFunction newFunc,
|*								   GemTimerFunction *oldFunc,
|*								   int16_t *mpt
|*
\*****************************************************************************/
void VDI::vex_timv(qintptr handle, int16_t enable)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		cm->updateTimerInterest(ws->handle(), enable);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vex_timv(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t enable		= ntohs(p[0]);

	vex_timv(ws->handle(), enable);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	int16_t ms = 50;		// or 20 times per second

	cm->clear();
	cm->append(ms);
	cm->setType(MSG_REPLY(ClientMsg::VEX_TIMV));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 17100: Set the event filter for a client.
\*****************************************************************************/
void VDI::setEventFilter(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t filter		= ntohs(p[0]);

	ws->setActiveEvents(filter);
	}

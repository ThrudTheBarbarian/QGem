#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 108: Set the end-style for lines.
|*
|* Original signature is: vsl_ends(int16_t handle, int16_t start, int16_t end);
|*
\*****************************************************************************/
void VDI::vsl_ends(qintptr handle, int16_t start, int16_t end)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((start < CAP_SQUARE) || (start > CAP_ROUND))
		start = CAP_SQUARE;
	if ((end < CAP_SQUARE) || (end > CAP_ROUND))
		end = CAP_SQUARE;

	if (ws != nullptr)
		{
		ws->setStartCap(start);
		ws->setEndCap(end);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsl_ends(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t start		= ntohs(p[0]);
	int16_t end			= ntohs(p[1]);

	vsl_ends(ws->handle(), start, end);
	}

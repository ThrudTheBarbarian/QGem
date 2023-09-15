
#include "clientmsg.h"
#include "connectionmgr.h"
#include "debug.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.4: Move the cursor up if possible.
|*
|* Original signature is: vs_clip(int16_t handle,
|*								  int16_t enableClip,
|*								  int16_t *pxy);
|*
|* pxy contains (in order) {left, top, right, bottom}
\*****************************************************************************/
void VDI::vs_clip(qintptr handle, int16_t enableClip, int16_t *pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (pxy != nullptr))
		{
		QRect r = {pxy[0], pxy[1], pxy[2] - pxy[0],  pxy[3] - pxy[1]};
		ws->setClip(r);
		ws->setEnableClip(enableClip > 0);
		}
	else if (ws == nullptr)
		{
		WARN("Cannot find workstation!");
		}
	else if (pxy == nullptr)
		{
		WARN("NULL cliprect given!");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_clip(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t enableClip	= ntohs(p[0]);
	int16_t *clip		= (int16_t *)(&(p[1]));

	for (int i=0; i<4; i++)
		clip[i] = ntohs(clip[i]);

	vs_clip(ws->handle(), enableClip, clip);
	}

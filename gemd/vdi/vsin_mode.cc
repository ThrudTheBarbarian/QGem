
#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 33: Set the input mode.
|*
|* Original signature is: vsin_mode(int16_t handle,
|*									int16_t device,
|*									int16_t mode);
|*
\*****************************************************************************/
void VDI::vsin_mode(qintptr handle, int16_t device, int16_t mode)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		if ((device >= INP_LOCATOR) && (device < INP_MAX))
			if ((mode == INPUT_REQUEST) || (mode == INPUT_SAMPLE))
				ws->setInputMode(device, mode);
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vsin_mode(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Decode the incoming mode value
	\**************************************************************************/
	const Payload &p	= cm->payload();
	int16_t device		= ntohs(p[0]);
	int16_t mode		= ntohs(p[1]);

	vsin_mode(ws->handle(), device, mode);
	}


#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 115: Query the input mode.
|*
|* Original signature is: vqin_mode(int16_t handle,
|*									int16_t device,
|*									int16_t *mode);
|*
|*
\*****************************************************************************/
void VDI::vqin_mode(qintptr handle, int16_t device, int16_t &mode)
	{
	ConnectionMgr *cm	= _screen->connectionManager();
	Workstation *ws		= cm->findWorkstationForHandle(handle);
	int modeType		= -1;

	if (ws != nullptr)
		modeType = ws->inputMode(device);

	mode = modeType;
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vqin_mode(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Decode the incoming mode value
	\**************************************************************************/
	const Payload &p	= cm->payload();
	int16_t device		= ntohs(p[0]);
	int16_t mode;

	vqin_mode(ws->handle(), device, mode);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(&mode, 1);
	cm->setType(MSG_REPLY(ClientMsg::VQIN_MODE));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

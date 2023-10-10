
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 37: Get the current fill style
|*
|* Original signature is: vqf_attributes(int16_t handle, int16_t *settings);
|*
\*****************************************************************************/
void VDI::vqf_attributes(qintptr handle, int16_t* settings)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (settings != nullptr))
		{
		settings[0] = ws->fillType();
		settings[1] = ws->fillColour();
		settings[2] = ws->fillStyle();
		settings[3] = ws->drawPerimeter() ? 1 : 0;
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqf_attributes(Workstation *ws, ClientMsg *cm)
	{
	int16_t data[4];
	vqf_attributes(ws->handle(), data);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(data, 4);
	cm->setType(MSG_REPLY(ClientMsg::VQF_ATTRIBUTES));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 36: Get the current marker-drawing style
|*
|* Original signature is: vqm_attributes(int16_t handle, int16_t *settings);
|*
\*****************************************************************************/
void VDI::vqm_attributes(qintptr handle, int16_t* settings)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (settings != nullptr))
		{
		settings[0] = ws->markerType();
		settings[1] = ws->markerColour();
		settings[2] = ws->wrMode();
		settings[3] = ws->markerHeight();
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqm_attributes(Workstation *ws, ClientMsg *cm)
	{
	int16_t data[4];
	vqm_attributes(ws->handle(), data);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(data, 4);
	cm->setType(MSG_REPLY(ClientMsg::VQM_ATTRIBUTES));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

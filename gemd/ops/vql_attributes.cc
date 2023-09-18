
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 35: Get the current line-drawing style
|*
|* Original signature is: vql_attributes(int16_t handle, int16_t *settings);
|*
\*****************************************************************************/
void VDI::vql_attributes(qintptr handle, int16_t* settings)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (settings != nullptr))
		{
		settings[0] = ws->lineType();
		settings[1] = ws->lineColour();
		settings[2] = ws->wrMode();
		settings[3] = ws->lineWidth();
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vql_attributes(Workstation *ws, ClientMsg *cm)
	{
	int16_t data[4];
	vql_attributes(ws->handle(), data);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(data, 4);
	cm->setType(MSG_REPLY(ClientMsg::VQL_ATTRIBUTES));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

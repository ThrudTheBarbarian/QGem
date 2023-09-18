
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 38: Get the current text style
|*
|* Original signature is: vqt_attributes(int16_t handle, int16_t *settings);
|*
\*****************************************************************************/
void VDI::vqt_attributes(qintptr handle, int16_t* settings)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (settings != nullptr))
		{
		settings[0] = ws->fontId();
		settings[1] = ws->textColour();
		settings[2] = ws->textRotation();
		settings[3] = ws->horizontalTextAlignment();
		settings[4] = ws->verticalTextAlignment();
		settings[5] = ws->wrMode();
		settings[6] = ws->fm()->maxWidth();
		settings[7] = ws->fm()->height();
		settings[8] = settings[6];
		settings[9] = settings[7];
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqt_attributes(Workstation *ws, ClientMsg *cm)
	{
	int16_t data[10];
	vqt_attributes(ws->handle(), data);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(data, 10);
	cm->setType(MSG_REPLY(ClientMsg::VQT_ATTRIBUTES));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 26: Query the RGB values of a colour index. Note that the flag is
|*            irrelevant for us, we use truecolour values
|*
|* Original signature is:
|*
|*  vq_color(int16_t handle, int16_t idx, int16_t flag, int16_t* rgb);
|*
\*****************************************************************************/
void VDI::vq_color(qintptr handle, int16_t idx, int16_t* rgb)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QColor colour = {0,0,0,0};

	if ((ws != nullptr) && (idx >= 0) && (idx < 256))
		colour = ws->colour(idx);

	rgb[0] = colour.red();
	rgb[1] = colour.green();
	rgb[2] = colour.blue();
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t idx			= ntohs(p[0]);

	int16_t rgb[3];
	vq_color(ws->handle(), idx, rgb);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(rgb, 3);
	cm->setType(MSG_REPLY(ClientMsg::VQ_COLOR));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

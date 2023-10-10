
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 2026: Query the RGB values of a colour palette.
|*
\*****************************************************************************/
void VDI::vq_colours(qintptr handle, int16_t* rgb)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	ws->colourPalette(rgb);
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_colours(Workstation *ws, ClientMsg *cm)
	{
	int16_t rgb[256*3];
	vq_colours(ws->handle(), rgb);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(rgb, 3*256);
	cm->setType(MSG_REPLY(ClientMsg::VQ_COLOURS));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

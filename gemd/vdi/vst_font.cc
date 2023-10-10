#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 21: Set the font to use.
|*
|* Original signature is: vsm_height(int16_t handle, int16_t height);
|*
\*****************************************************************************/
int VDI::vst_font(qintptr handle, int16_t fontId)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QFont *font = FontMgr::sharedInstance().fetch(fontId);
	if (font == nullptr)
		fontId = 0;

	if (ws != nullptr)
		{
		ws->setFont(fontId);
		}
	return fontId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_font(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t fontId		= ntohs(p[0]);

	fontId = vst_font(ws->handle(), fontId);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(fontId);
	cm->setType(MSG_REPLY(ClientMsg::VST_FONT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

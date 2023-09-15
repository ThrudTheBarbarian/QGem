#include <QPainter>

#include "clientmsg.h"
#include "fontmgr.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 130: Get the name of a font at an index. Font name can be up to
|* 32 chars
|*
|* Original signature is: vqt_name(int16_t handle, int16_t idx, char *name);
|*
\*****************************************************************************/
int16_t VDI::vqt_name(qintptr handle, int16_t fontId, char *name)
	{
	(void)handle;

	QFont *font = FontMgr::sharedInstance().fetch(fontId);
	if (font == nullptr)
		{
		fontId = 0;
		name[0] = '\0';
		}
	else
		{
		std::string s		= font->family().toStdString();
		const char *srcName = s.c_str();
		strncpy(name,srcName, 32);
		if (s.length() > 31)
			name[31] = '\0';
		}
	return fontId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqt_name(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t fontId		= ntohs(p[0]);
	char name[64];

	fontId = vqt_name(ws->handle(), fontId, name);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(fontId);
	int bytes = strlen(name);
	cm->append((uint8_t *)name, bytes+1);

	cm->setType(MSG_REPLY(ClientMsg::VQT_NAME));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

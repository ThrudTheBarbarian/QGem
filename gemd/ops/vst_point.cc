#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 107: Set the height of text in points.
|*
|* Original signature is: void vst_point(int16_t handle, int16_t height,
|*						 				 int16_t* charW, int16_t* charH,
|*										 int16_t* cellW, int16_t* cellH);
|*
\*****************************************************************************/
void VDI::vst_point(qintptr handle, int16_t height, int16_t& charWidth,
					int16_t& charHeight, int16_t& cellWidth,
					int16_t &cellHeight)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	// Assuming a 96 dpi screen, and one point = 1/72"
	height = (height * 72) / 96;

	height = (height < 1) ? 1
		   : (height > 512) ? 512
		   : height;

	if (ws != nullptr)
		{
		ws->setTextHeight(height);

		charWidth	= cellWidth		= (ws->fm()->maxWidth() * 96) / 72;
		charHeight	= cellHeight	= (ws->fm()->height() * 96) / 72;
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_point(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t height		= ntohs(p[0]);
	int16_t charW		= 0;
	int16_t charH		= 0;
	int16_t cellW		= 0;
	int16_t cellH		= 0;

	vst_point(ws->handle(), height, charW, charH, cellW, cellH);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(charW);
	cm->append(charH);
	cm->append(cellW);
	cm->append(charH);
	cm->setType(MSG_REPLY(ClientMsg::VST_POINT));
	}

#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 19: Set the height of marker in pixels.
|*
|* Original signature is: vsm_height(int16_t handle, int16_t height);
|*
\*****************************************************************************/
void VDI::vst_height(qintptr handle, int16_t height, int16_t& charWidth,
					 int16_t& charHeight, int16_t& cellWidth,
					 int16_t &cellHeight)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	height = (height < 1) ? 1
			 : (height > 512) ? 512
			 : height;

	if (ws != nullptr)
		{
		ws->setTextHeight(height);

		charWidth	= cellWidth		= ws->fm()->maxWidth();
		charHeight	= cellHeight	= ws->fm()->height();
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_height(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t height		= p[0];
	int16_t charW		= 0;
	int16_t charH		= 0;
	int16_t cellW		= 0;
	int16_t cellH		= 0;

	vst_height(ws->handle(), height, charW, charH, cellW, cellH);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(charW);
	cm->append(charH);
	cm->append(cellW);
	cm->append(charH);
	cm->setType(MSG_REPLY(ClientMsg::VST_HEIGHT));

	}

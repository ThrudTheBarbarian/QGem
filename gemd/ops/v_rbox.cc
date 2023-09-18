#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 11.8	Draw a rounded rect
|*
|* Original signature is: v_rbox(int16_t handle, int16_t* pxy);
|*
\*****************************************************************************/
void VDI::v_rbox(qintptr handle, int16_t* pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QPainter painter(_img);

	if (ws != nullptr)
		{
		QPen pen;

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		ws->setWritingMode(painter);

		/*********************************************************************\
		|* Figure out the parameters
		\*********************************************************************/
		int16_t x = pxy[0];
		int16_t y = pxy[1];
		int16_t w = pxy[2] - x;
		int16_t h = pxy[3] - y;

		/*********************************************************************\
		|* Draw the path perimeter, set-up the pen
		\*********************************************************************/
		ws->setupPenForLine(pen);
		painter.setPen(pen);
		painter.drawRoundedRect(x, y, w, h, 25, 25, Qt::RelativeSize);
		}
	}

/*****************************************************************************\
|* And from the socket interface
|*
|* Opcode 11.2:	Draw an arc
\*****************************************************************************/
void VDI::v_rbox(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t coords[4];

	for (int i=0; i<4; i++)
		coords[i] = ntohs(p[i]);

	v_rbox(ws->handle(), coords);
	}



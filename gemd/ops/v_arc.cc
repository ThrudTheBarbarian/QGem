#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 11.2	Draw an arc
|*
|* Original signature is: v_arc(int16_t handle, int16_t x, int16_t y,
|*								int16_t radius,
|*						        int16_t begang, int16_t endang);
|*
\*****************************************************************************/
void VDI::v_arc(qintptr handle, int16_t x, int16_t y, int16_t radius,
				int16_t start, int16_t end)
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
		|* Create the path that we'll draw
		\*********************************************************************/
		QPainterPath path;
		int16_t X = x - radius;
		int16_t Y = y - radius;
		int16_t d = radius * 2;
		qreal   s = start / 10.0;
		qreal   e = end / 10.0 - s;

		path.arcMoveTo(X, Y, d, d, s);
		path.arcTo(X, Y, d, d, s, e);

		/*********************************************************************\
		|* Draw the path perimeter, set-up the pen
		\*********************************************************************/
		ws->setupPenForLine(pen);
		painter.setPen(pen);
		painter.drawPath(path);
		}
	}

/*****************************************************************************\
|* And from the socket interface
|*
|* Opcode 11.2:	Draw an arc
\*****************************************************************************/
void VDI::v_arc(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);
	int16_t radius		= ntohs(p[2]);
	int16_t start		= ntohs(p[3]);
	int16_t end			= ntohs(p[4]);

	v_arc(ws->handle(), x, y, radius, start, end);
	}



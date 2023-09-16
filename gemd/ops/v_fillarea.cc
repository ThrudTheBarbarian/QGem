#include <QColor>#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 9: Fill a polygon.
|*
|* Original signature is: v_fillarea(int16_t handle, int16_t numPts, int16_t*pxy);
|*
\*****************************************************************************/
void VDI::v_fillarea(qintptr handle, int16_t num, int16_t*pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QPainter painter(_img);

	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForFill(pen);
		painter.setPen(pen);

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Create the polygon that we'll fill / draw
		\*********************************************************************/
		QPolygonF poly;
		int idx = 0;
		for (int i=0; i<num; i++)
			{
			int16_t x = pxy[idx++];
			int16_t y = pxy[idx++];
			poly << QPoint(x, y);
			}
		if (!poly.isClosed())
			poly << poly.first();

		/*********************************************************************\
		|* If the fill-mode is hollow, we don't fill
		\*********************************************************************/
		if (ws->fillType() != FIS_HOLLOW)
			{
			QPainterPath path;
			path.addPolygon(poly);
			painter.fillPath(path, pen.brush());
			}

		/*********************************************************************\
		|* If we're going to draw the perimeter, re-set-up the pen
		\*********************************************************************/
		if (ws->drawPerimeter())
			{
			ws->setupPenForLine(pen);
			painter.setPen(pen);
			painter.drawPolygon(poly);
			}
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_fillarea(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= ntohs(p[0]);
	int16_t *pxy		= (int16_t *)(&(p[1]));

	for (int i=0; i<num*2; i++)
		pxy[i] = ntohs(pxy[i]);

	v_fillarea(ws->handle(), num, pxy);
	}


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
|* Opcode 9:	Fill a polygon.			[type=0] [pxy=...]
|* Opcode 11.1	Fill a rectangle		[type=1] [pxy=x0,y0,x1,y1]
|* Opcode 11.3	Fill an arc				[type=2] [pxy=x,y,r,begin,end]
|* Opcode 11.4  Fill a circle			[type=4] [pxy=x,y,r]
|* Opcode 11.5	Fill an ellipse			[type=5] [pxy=x,y,rx,ry]
|* Opcode 11.7  Fill an elliptical pie	[type=7] [pxy=x,y,xr,yr,begin,end]
|* Opcode 11.9	Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
|*
|* Original signature is: v_fillarea(int16_t handle, int16_t numPts, int16_t*pxy);
|*
|* re
\*****************************************************************************/
void VDI::v_fillarea(qintptr handle, FillType type, int16_t num, int16_t*pxy)
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
		|* Create the path that we'll fill / draw
		\*********************************************************************/
		QPainterPath path;
		switch (type)
			{
			case POLY:
				{
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

				path.addPolygon(poly);
				break;
				}

			case RECT:
				if (num == 4)
					{
					int16_t w = pxy[2] - pxy[0];
					int16_t h = pxy[3] - pxy[1];
					path.addRect(pxy[0], pxy[1], w, h);
					}
				break;

			case PIE:
				if (num == 5)
					{
					int16_t x = pxy[0] - pxy[2];
					int16_t y = pxy[1] - pxy[2];
					int16_t d = pxy[2] * 2;
					qreal   s = pxy[3] / 10.0;
					qreal   e = pxy[4] / 10.0 - s;

					path.moveTo(pxy[0], pxy[1]);
					path.arcTo(x, y, d, d, s, e);
					path.lineTo(pxy[0], pxy[1]);
					}
				break;

			case CIRCLE:
				if (num == 3)
					{
					int16_t x = pxy[0] - pxy[2];
					int16_t y = pxy[1] - pxy[2];
					int16_t d = pxy[2] * 2;
					path.addEllipse(x, y, d, d);
					}
				break;

			case ELLIPSE:
				if (num == 4)
					{
					int16_t x = pxy[0] - pxy[2];
					int16_t y = pxy[1] - pxy[3];
					int16_t w = pxy[2] * 2;
					int16_t h = pxy[3] * 2;
					path.addEllipse(x, y, w, h);
					}
				break;

			case ELLIPTICAL_PIE:
				if (num == 6)
					{
					int16_t x = pxy[0] - pxy[2];
					int16_t y = pxy[1] - pxy[3];
					int16_t w = pxy[2] * 2;
					int16_t h = pxy[3] * 2;
					qreal   s = pxy[4] / 10.0;
					qreal   e = pxy[5] / 10.0 - s;

					path.moveTo(pxy[0], pxy[1]);
					path.arcTo(x, y, w, h, s, e);
					path.lineTo(pxy[0], pxy[1]);
					}
				break;

			case ROUNDED_RECT:
				if (num == 4)
					{
					int16_t w = pxy[2] - pxy[0];
					int16_t h = pxy[3] - pxy[1];
					path.addRoundedRect(pxy[0], pxy[1], w, h,
										15, 15, Qt::RelativeSize);
					}
				break;
			}

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
			painter.fillPath(path, pen.brush());
			}

		/*********************************************************************\
		|* If we're going to draw the perimeter, re-set-up the pen
		\*********************************************************************/
		if (ws->drawPerimeter())
			{
			ws->setupPenForLine(pen);
			painter.setPen(pen);
			painter.drawPath(path);
			}
		}
	}

/*****************************************************************************\
|* And from the socket interface
|*
|* Opcode 9:	Fill a polygon.			[type=0] [pxy=...]
\*****************************************************************************/
void VDI::v_fillarea(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= ntohs(p[0]);
	int16_t *pxy		= (int16_t *)(&(p[1]));

	for (int i=0; i<num*2; i++)
		pxy[i] = ntohs(pxy[i]);

	v_fillarea(ws->handle(), POLY, num, pxy);
	}



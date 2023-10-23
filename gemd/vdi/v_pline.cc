#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Forward declarations...
\*****************************************************************************/
static void _drawArrow(QPainter *painter, QPointF p1, QPointF p2, int width);

/*****************************************************************************\
|* Opcode 6: Draw a poly-line.
|*
|* Original signature is: v_pline(int16_t handle, int16_t numPts, int16_t*pxy);
|*
\*****************************************************************************/
void VDI::v_pline(qintptr handle, int16_t num, int16_t*pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);


	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForLine(pen);

		QList<QPointF> pts;
		int idx = 0;
		for (int i=0; i<num; i++)
			{
			QPointF p(pxy[idx], pxy[idx+1]);
			pts << p;
			idx += 2;
			}
		QPainter painter(_img);

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		ws->setWritingMode(painter);

		bool penNeedsReset = false;
		int W	= ws->lineWidth() * 3 + 3;

		if (ws->startCap() == CAP_ROUND)
			{
			penNeedsReset = true;
			pen.setCapStyle(Qt::RoundCap);
			}
		else if (ws->endCap() == CAP_ROUND)
			{
			penNeedsReset = true;
			pen.setCapStyle(Qt::RoundCap);
			}

		painter.setPen(pen);
		if ((num == 2) && (pts[0] == pts[1]))
			painter.drawPoint(pts[0]);
		else
			painter.drawPolyline(pts.constData(), num);

		if (num > 1)
			{
			if (ws->startCap() == CAP_ARROW)
				{
				penNeedsReset = true;
				pen.setStyle(Qt::SolidLine);
				painter.setPen(pen);
				_drawArrow(&painter, pts[0], pts[1], W);
				}
			if (ws->endCap() == CAP_ARROW)
				{
				penNeedsReset = true;
				pen.setStyle(Qt::SolidLine);
				painter.setPen(pen);
				_drawArrow(&painter, pts[num-1], pts[num-2], W);
				}
			}
		if (penNeedsReset)
			ws->setupPenForLine(pen);

		painter.setClipping(false);
		}

	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_pline(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= ntohs(p[0]);
	int16_t *pxy		= (int16_t *)(&(p[1]));

	for (int i=0; i<num*2; i++)
		pxy[i] = ntohs(pxy[i]);

	v_pline(ws->handle(), num, pxy);
	}

/*****************************************************************************\
|* Draw an arrow-head at the end of a line
\*****************************************************************************/
void _drawArrow(QPainter *painter, QPointF p1, QPointF p2, int width)
	{
	double angle = std::atan2(-(p1.y() - p2.y()), p1.x()-p2.x());

	QPointF arrowP1 = p1 + QPointF(-sin(angle + M_PI / 3) * width,
								   -cos(angle + M_PI / 3) * width);
	QPointF arrowP2 = p1 + QPointF(-sin(angle + M_PI - M_PI / 3) * width,
								   -cos(angle + M_PI - M_PI / 3) * width);


	QList<QPointF> pts;
	pts << arrowP1 << p1 << arrowP2;
	painter->drawPolyline(pts.constData(), 3);
	}

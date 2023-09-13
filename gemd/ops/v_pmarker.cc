#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 7: Draw a poly-marker.
|*
|* Original signature is: v_pmarker(int16_t handle, int16_t numPts, int16_t*pxy);
|*
\*****************************************************************************/
void VDI::v_pmarker(qintptr handle, int16_t num, int16_t*pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QPainter painter(_img);

	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForMarker(pen);
		painter.setPen(pen);
		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		int idx   = 0;
		int delta = (ws->markerHeight() - 1)/2;

		QList<QPoint> pts;
		for (int i=0; i<num; i++)
			{
			int16_t x = pxy[idx++];
			int16_t y = pxy[idx++];

			switch (ws->markerType())
				{
				case MRKR_DOT:
					painter.drawPoint(x,y);
					break;

				case MRKR_PLUS:
					pts << QPoint(x, y-delta) << QPoint(x, y+delta)
						<< QPoint(x-delta, y) << QPoint(x+delta, y);
					painter.drawLines(pts);
					break;

				case MRKR_ASTERISK:
					pts << QPoint(x, y-delta) << QPoint(x, y+delta)
						<< QPoint(x-delta, y-delta) << QPoint(x+delta,y+delta)
						<< QPoint(x+delta, y-delta) << QPoint(x-delta,y+delta);
					painter.drawLines(pts);
					break;

				case MRKR_BOX:
					pts << QPoint(x-delta, y-delta) << QPoint(x+delta, y-delta)
						<< QPoint(x+delta, y+delta) << QPoint(x-delta, y+delta)
						<< QPoint(x-delta, y-delta);
					painter.drawPolyline(pts);
					break;

				case MRKR_CROSS:
					pts << QPoint(x-delta, y-delta) << QPoint(x+delta,y+delta)
						<< QPoint(x+delta, y-delta) << QPoint(x-delta,y+delta);
					painter.drawLines(pts);
					break;

				case MRKR_DIAMOND:
					pts << QPoint(x, y-delta) << QPoint(x+delta, y)
						<< QPoint(x, y+delta) << QPoint(x-delta, y)
						<< QPoint(x, y-delta);
					painter.drawPolyline(pts);
					break;

				default:
					painter.drawEllipse(x, y, delta, delta);
				}
			}
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_pmarker(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= p[0];
	int16_t *pxy		= (int16_t *)(&(p[1]));

	v_pmarker(ws->handle(), num, pxy);
	}


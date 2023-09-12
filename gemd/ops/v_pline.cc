#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

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
		ws->setupPen(pen);

		QList<QPoint> pts;
		int idx = 0;
		for (int i=0; i<num; i++)
			{
			QPoint p(pxy[idx], pxy[idx+1]);
			pts << p;
			idx += 2;
			}

		QPainter painter(_img);
		painter.setPen(pen);
		painter.drawPolyline(pts.constData(), num);
		}

	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_pline(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= p[0];
	int16_t *pxy		= (int16_t *)(&(p[1]));

	v_pline(ws->handle(), num, pxy);
	}

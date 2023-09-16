#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* From the socket interface
|*
|*  11.9: Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
\*****************************************************************************/
void VDI::v_rfbox(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t num			= ntohs(p[0]);
	int16_t *pxy		= (int16_t *)(&(p[1]));

	for (int i=0; i<num; i++)
		pxy[i] = ntohs(pxy[i]);

	v_fillarea(ws->handle(), ROUNDED_RECT, num, pxy);
	}

#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* From the socket interface
|*
|*  11.10: Draw justfied text
\*****************************************************************************/
void VDI::v_justified(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);
	int16_t w			= ntohs(p[2]);

	QByteArray ba;
	cm->fetchData(3, ba);

	v_gtext(ws->handle(), x, y, w, ba.data());
	}

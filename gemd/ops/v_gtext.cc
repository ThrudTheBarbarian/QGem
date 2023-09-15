#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "fontmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

#define VALIGN_MASK (Qt::AlignTop | Qt::AlignBottom | Qt::AlignBaseline)

/*****************************************************************************\
|* Opcode 8: Draw graphic text
|*
|* Original signature is: v_gtext(int16_t hndl, int16_t x, int16_t y, char *s);
|*
\*****************************************************************************/
void VDI::v_gtext(qintptr handle, int16_t x, int16_t y, char *txt)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForText(pen);

		QPainter painter(_img);
		painter.setPen(pen);

		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		QFont font				= QFont(ws->currentFont(), _img);
		QFontMetrics *metrics	= ws->fm();

		int effects = ws->textEffect();
		font.setBold(effects & TXT_BOLD);
		font.setItalic(effects & TXT_ITALIC);
		font.setUnderline(effects & TXT_UNDERLINE);

		font.setPixelSize(ws->textHeight());

		painter.setFont(font);

		// Y position is used as baseline, so if we have different vrtical
		// alignment, we have to calculate that ourselves
		switch (ws->textAlign() & VALIGN_MASK)
			{
			case Qt::AlignTop:
				y -= metrics->ascent();
				break;
			case Qt::AlignBottom:
				y += metrics->descent();
				break;
			}

		int angle = ws->textRotation();
		if (angle != 0)
			{
			painter.translate(x, y);
			painter.rotate(angle);
			painter.translate(-x, -y);
			}

		if (effects & TXT_OUTLINE)
			{
			QPainterPath path;
			path.addText(QPoint(x,y), font, txt);
			painter.strokePath(path.simplified(), pen);
			}
		else
			painter.drawText(x, y, txt);
		}

	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_gtext(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);

	QByteArray ba;
	cm->fetchData(2, ba);

	v_gtext(ws->handle(), x, y, ba.data());
	}


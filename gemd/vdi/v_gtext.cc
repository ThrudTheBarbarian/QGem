#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 8: Draw graphic text
|*
|* Original signature is: v_gtext(int16_t hndl, int16_t x, int16_t y, char *s);
|*
\*****************************************************************************/
void VDI::v_gtext(qintptr handle, int16_t x, int16_t y, int16_t w, char *txt)
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

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		ws->setWritingMode(painter);


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
		switch (ws->textVAlign())
			{
			case TA_TOP:
				y -= metrics->ascent();
				break;
			case TA_BOTTOM:
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

		if (w == 0)
			{
			if (effects & TXT_OUTLINE)
				{
				QPainterPath path;
				path.addText(QPoint(x,y), font, txt);
				painter.strokePath(path.simplified(), pen);
				}
			else
				painter.drawText(x, y, txt);
			}
		else
			{
			int flags = 0;
			switch (ws->textHAlign())
				{
				case TA_FULL:
					flags	= Qt::TextJustificationForced | Qt::AlignJustify;
					break;
				case TA_CENTER:
					flags = Qt::AlignCenter;
					break;
				case TA_RIGHT:
					flags = Qt::AlignRight;
					break;
				default:
					flags = Qt::AlignLeft;
					break;
				}

			int h = ws->fm()->height();
			painter.drawText(x, y, w, h, flags, txt);
			}
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

	v_gtext(ws->handle(), x, y, 0, ba.data());
	}


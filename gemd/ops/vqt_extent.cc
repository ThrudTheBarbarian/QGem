#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 8: Draw graphic text
|*
|* Original signature is: v_gtext(int16_t hndl, int16_t x, int16_t y, char *s);
|*
\*****************************************************************************/
void VDI::vqt_extent(qintptr handle, char *txt, int16_t *pxy)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		QPen pen;
		ws->setupPenForText(pen);

		QPainter painter(_img);
		painter.setPen(pen);

		/*********************************************************************\
		|* Set up the font
		\*********************************************************************/
		QFont font				= QFont(ws->currentFont(), _img);
		int effects				= ws->textEffect();
		font.setBold(effects & TXT_BOLD);
		font.setItalic(effects & TXT_ITALIC);
		font.setUnderline(effects & TXT_UNDERLINE);

		font.setPixelSize(ws->textHeight());

		painter.setFont(font);
		QFontMetrics *metrics	= ws->fm();
		QRect bounds			= metrics->boundingRect(txt);

		int angle = ws->textRotation();
		if (angle != 0)
			{
			painter.rotate(angle);
			bounds = painter.transform().mapRect(bounds);
			}

		if (pxy)
			{
			pxy[0] = bounds.bottomLeft().x();
			pxy[1] = bounds.bottomLeft().y();
			pxy[2] = bounds.bottomRight().x();
			pxy[3] = bounds.bottomRight().y();
			pxy[4] = bounds.topRight().x();
			pxy[5] = bounds.topRight().y();
			pxy[6] = bounds.topLeft().x();
			pxy[7] = bounds.topLeft().y();
			}
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqt_extent(Workstation *ws, ClientMsg *cm)
	{
	int16_t pxy[8];
	QByteArray ba;
	cm->fetchData(0, ba);

	vqt_extent(ws->handle(), ba.data(), pxy);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(pxy, 8);
	cm->setType(MSG_REPLY(ClientMsg::VQT_EXTENT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


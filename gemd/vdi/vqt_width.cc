#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 117: Get char extents
|*
|* Original signature is: void vqt_width(int16_t handle,
|*										 char c,
|*										 int16_t *cellWidth,
|*										 int16_t *leftDelta,
|*										 int16_t * rightDelta)
|*
|*
\*****************************************************************************/
void VDI::vqt_width(qintptr handle,
					char c,
					int16_t& cellWidth,
					int16_t& leftDelta,
					int16_t& rightDelta)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		/*********************************************************************\
		|* Set up the font
		\*********************************************************************/
		QFont font				= QFont(ws->currentFont());
		int effects				= ws->textEffect();
		font.setBold(effects & TXT_BOLD);
		font.setItalic(effects & TXT_ITALIC);
		font.setUnderline(effects & TXT_UNDERLINE);
		font.setPixelSize(ws->textHeight());

		/*********************************************************************\
		|* Get the metrics
		\*********************************************************************/
		QFontMetrics metrics(font);
		char txt[4];
		snprintf(txt, 3, "%c", c);
		QRect letter = metrics.boundingRect(txt);
		QRect space  = metrics.boundingRect(".");


		/*********************************************************************\
		|* Add up the values and return
		\*********************************************************************/
		cellWidth	= letter.width() + space.width();
		leftDelta	= space.width()/2;
		rightDelta	= leftDelta;
		}
	else
		cellWidth	= leftDelta = rightDelta = 0;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vqt_width(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Decode the incoming letter value
	\**************************************************************************/
	const Payload &p	= cm->payload();
	int16_t letter		= ntohs(p[0]);

	int16_t cellWidth, leftDelta, rightDelta;
	vqt_width(ws->handle(), (char)letter, cellWidth, leftDelta, rightDelta);

	/**************************************************************************\
	|* Construct the reply
	\**************************************************************************/
	cm->clear();
	cm->append(cellWidth);
	cm->append(leftDelta);
	cm->append(rightDelta);
	cm->setType(MSG_REPLY(ClientMsg::VQT_WIDTH));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


#include <QFontMetrics>

#include "clientmsg.h"
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 5.1: Query the number of character cells on the alpha display.
|*
|* Original signature is:
|*
|*  vq_chcells(int16_t handle, int16_t& rows, int16_t& columns);
|*
\*****************************************************************************/
void VDI::vq_chcells(qintptr handle, int16_t& rows, int16_t& columns)
	{
	if (handle != 0)
		{
		WARN("Non-screen devices currently unsupported");
		}
	else
		{
		QFontMetrics metrics(_systemFont);
		QRect bounds	= metrics.boundingRect("W");
		_charHeight		= bounds.height();
		_charWidth		= bounds.width();
		rows			= _screen->height() / _charHeight;
		columns			= _screen->width() / _charWidth;
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_chcells(Workstation *ws, ClientMsg *cm)
	{
	int16_t rows, cols;
	vq_chcells(0, rows, cols);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(rows);
	cm->append(cols);
	cm->setType(ClientMsg::MSG_REPLY_OFFSET + ClientMsg::VQ_CHCELLS);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

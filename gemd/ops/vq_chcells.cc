#include <QFontMetrics>

#include "clientmsg.h"
#include "connectionmgr.h"
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
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);
	if (ws != nullptr)
		{
		QFontMetrics *fm	= ws->fm();
		QRect bounds		= fm->boundingRect("W");
		_charHeight			= bounds.height();
		_charWidth			= bounds.width();
		rows				= _screen->height() / _charHeight;
		columns				= _screen->width() / _charWidth;
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_chcells(Workstation *ws, ClientMsg *cm)
	{
	int16_t rows = 0;
	int16_t cols = 0;
	vq_chcells(0, rows, cols);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(rows);
	cm->append(cols);
	cm->setType(MSG_REPLY(ClientMsg::VQ_CHCELLS));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

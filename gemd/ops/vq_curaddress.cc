
#include "clientmsg.h"
#include "debug.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.15: Get the current cursor position.
|*
|* Original signature is: vq_curaddress(int16_t handle,
|*									    int16_t *row,
|*										int16_t *col);
|*
\*****************************************************************************/
void VDI::vq_curaddress(qintptr handle, int16_t& row, int16_t& col)
	{
	if (handle == 0)
		{
		col = _cursorX;
		row = _cursorY;
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_curaddress(Workstation *ws, ClientMsg *cm)
	{
	int16_t row, col;
	vq_curaddress(0, row, col);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(row);
	cm->append(col);
	cm->setType(ClientMsg::MSG_REPLY_OFFSET + ClientMsg::VQ_CURADDRESS);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

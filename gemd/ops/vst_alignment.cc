
#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 39: Request the text alignment and get the actual set values
|*
|* Original signature is: vst_lignment(int16_t handle,
|*									   int16_t hIn,
|*									   int16_t vIn,
|*									   int16_t *hOut,
|*									   int16_t *vOut);
|*
\*****************************************************************************/
void VDI::vst_alignment(qintptr handle, int16_t  hIn,  int16_t vIn,
				   int16_t& hOut, int16_t& vOut)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		int flags = 0;
		switch (hIn)
			{
			case 1:
				flags = Qt::AlignHCenter;
				hOut = hIn;
				break;
			case 2:
				flags = Qt::AlignRight;
				hOut = hIn;
				break;
			default:
				flags = Qt::AlignLeft;
				hOut = 0;
				break;
			}

		switch (vIn)
			{
			case 1:
			case 2:
				flags |= Qt::AlignTop;
				vOut = 2;
				break;

			case 3:
			case 4:
				flags |= Qt::AlignBottom;
				vOut = 3;
				break;

			default:
				flags |= Qt::AlignBaseline;
				vOut = 0;
				break;
			}
		ws->setTextAlign(flags);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vst_alignment(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();

	/**************************************************************************\
	|* Get the request
	\**************************************************************************/
	int16_t hIn = p[0];
	int16_t vIn = p[1];

	int16_t hOut = 0;
	int16_t vOut = 0;
	vst_alignment(ws->handle(), hIn, vIn, hOut, vOut);

	/**************************************************************************\
	|* Construct the reply
	\**************************************************************************/
	cm->clear();
	cm->append(hOut);
	cm->append(vOut);
	cm->setType(MSG_REPLY(ClientMsg::VST_ALIGNMENT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

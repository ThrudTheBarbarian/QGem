
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
			case ALGN_CENTER:
				flags = Qt::AlignHCenter;
				hOut = hIn;
				break;
			case ALGN_RIGHT:
				flags = Qt::AlignRight;
				hOut = hIn;
				break;
			default:
				flags = Qt::AlignLeft;
				hOut = ALGN_LEFT;
				break;
			}

		switch (vIn)
			{
			case ALGN_HALF:
			case ALGN_ASCENT:
			case ALGN_TOP:
				flags |= Qt::AlignTop;
				vOut = ALGN_TOP;
				break;

			case ALGN_BOTTOM:
			case ALGN_DESCENT:
				flags |= Qt::AlignBottom;
				vOut = ALGN_BOTTOM;
				break;

			default:
				flags |= Qt::AlignBaseline;
				vOut = ALGN_BASELINE;
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
	int16_t hIn = ntohs(p[0]);
	int16_t vIn = ntohs(p[1]);

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

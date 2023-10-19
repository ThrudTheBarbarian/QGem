
#include "clientmsg.h"
#include "connectionmgr.h"
#include "gem.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 39: Request the text alignment and get the actual set values
|*
|* Original signature is: vst_alignment(int16_t handle,
|*									    int16_t hIn,
|*									    int16_t vIn,
|*									    int16_t *hOut,
|*									    int16_t *vOut);
|*
\*****************************************************************************/
void VDI::vst_alignment(qintptr handle, int16_t  hIn,  int16_t vIn,
				   int16_t& hOut, int16_t& vOut)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		switch (hIn)
			{
			case TA_CENTER:
			case TA_RIGHT:
			case TA_FULL:
				ws->setTextHAlign(hIn);
				hOut = hIn;
				break;
			default:
				ws->setTextHAlign(TA_LEFT);
				hOut = TA_LEFT;
				break;
			}

		switch (vIn)
			{
			case TA_HALF:
			case TA_ASCENT:
			case TA_TOP:
				ws->setTextVAlign(TA_TOP);
				vOut = TA_TOP;
				break;

			case TA_BOTTOM:
			case TA_DESCENT:
				ws->setTextVAlign(TA_BOTTOM);
				vOut = TA_BOTTOM;
				break;

			default:
				ws->setTextVAlign(TA_BASE);
				vOut = TA_BASE;
				break;
			}
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

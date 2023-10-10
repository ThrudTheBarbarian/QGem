
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
			case ALGN_CENTER:
			case ALGN_RIGHT:
			case ALGN_FULL:
				ws->setTextHAlign(hIn);
				hOut = hIn;
				break;
			default:
				ws->setTextHAlign(ALGN_LEFT);
				hOut = ALGN_LEFT;
				break;
			}

		switch (vIn)
			{
			case ALGN_HALF:
			case ALGN_ASCENT:
			case ALGN_TOP:
				ws->setTextVAlign(ALGN_TOP);
				vOut = ALGN_TOP;
				break;

			case ALGN_BOTTOM:
			case ALGN_DESCENT:
				ws->setTextVAlign(ALGN_BOTTOM);
				vOut = ALGN_BOTTOM;
				break;

			default:
				ws->setTextVAlign(ALGN_BASELINE);
				vOut = ALGN_BASELINE;
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

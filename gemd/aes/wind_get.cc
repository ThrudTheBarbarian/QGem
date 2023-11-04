
#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 6608: Get info about a window
\*****************************************************************************/
int16_t	AES::wind_get(qintptr handle, int16_t windowId, int16_t sub,
					   int16_t& x1, int16_t& x2, int16_t& x3, int16_t& x4)
	{
	int16_t ok = 0;

	ConnectionMgr *cm	= _vdi->screen()->connectionManager();
	Workstation *ws		= cm->findWorkstationForHandle(handle);
	int position		= windowForId(windowId);

	if ((ws != nullptr) && (position >= 0))
		{
		GWindow& win		= _windowList[position];
		if (win.handle == handle)
			{
			switch (sub)
				{
				case WF_FIRSTXYWH:
					win.xywhId = 0;
					/* fall through */

				case WF_NEXTXYWH:
					if (win.xywhId < win.rectList.size())
						{
						QRect rect = win.rectList.at(win.xywhId);
						x1 = rect.x();
						x2 = rect.y();
						x3 = rect.width();
						x4 = rect.height();
						win.xywhId ++;
						}
					else
						x1 = x2 = x3 = x4 = 0;
					break;
				}
			}
		}

	return ok;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::wind_get(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t winId		= ntohs(p[0]);
	int16_t sub			= ntohs(p[1]);
	int16_t x[4];

	int16_t ok = wind_get(ws->handle(), sub, winId, x[0], x[1], x[2], x[3]);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->setType(MSG_REPLY(ClientMsg::AES_WIND_GET));
	cm->append(ok);
	cm->append(x, 4);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

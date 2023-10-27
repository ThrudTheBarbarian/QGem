
#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 6610: Open a previously-created window
\*****************************************************************************/
int16_t	AES::wind_open(qintptr handle, int16_t windowId,
					   int16_t x, int16_t y,
					   int16_t w, int16_t h)
	{
	int16_t ok = 0;

	ConnectionMgr *cm = _vdi->screen()->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (windowId < _windowList.size()))
		{
		GWindow & win = _windowList[windowId];
		if (win.handle == handle)
			{
			/*****************************************************************\
			|* If this is the "desktop", ignore the x,y,w,h and set them to the
			|* screen size, with kind = 0 if this is the first window
			\*****************************************************************/
			const AppContext& ctx = _apps.value(ws->handle());
			if (!(ctx.isDesktop) || (win.root != true))
				win.current = QRect(x,y,w,h).intersected(win.max);

			ok = 1;
			}
		}

	return ok;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::wind_open(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t winId		= ntohs(p[0]);
	int16_t x			= ntohs(p[1]);
	int16_t y			= ntohs(p[2]);
	int16_t w			= ntohs(p[3]);
	int16_t h			= ntohs(p[4]);

	int16_t ok = wind_open(ws->handle(), winId, x, y, w, h);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(ok);
	cm->setType(MSG_REPLY(ClientMsg::AES_WIND_OPEN));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}


#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

static int _currentWindowId = 1000;

/*****************************************************************************\
|* 6604: Create and register a window, parameters are max-size
|*
|* returns -1 or the window handle
\*****************************************************************************/
int16_t AES::wind_create(qintptr handle, int16_t kind,
						 int16_t x, int16_t y,
						 int16_t w, int16_t h)
	{
	int16_t winId = -1;

	ConnectionMgr *cm = _vdi->screen()->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		/*********************************************************************\
		|* Add to the window list
		\*********************************************************************/
		GWindow win =
			{
			.kind		= kind,
			.current	= QRect(x,y,w,h),
			.max		= QRect(x,y,w,h),
			.shown		= false,
			.root		= false,
			.handle		= handle
			};

		/*********************************************************************\
		|* If this is the "desktop", ignore the x,y,w,h and set them to the
		|* screen size, with kind = 0 if this is the first window
		\*********************************************************************/
		const AppContext& ctx = _apps.value(ws->handle());
		if (ctx.isDesktop && (_windowList.size() == 0))
			{
			int W		= _vdi->screen()->width()-1;
			int H		= _vdi->screen()->height()-1;

			win.kind	= 0;
			win.current = QRect(0,0,W,H);
			win.max		= win.current;
			win.root	= true;
			}

		winId		 = _currentWindowId;
		win.windowId = _currentWindowId ++;
		_windowList.push_back(win);
		}

	return winId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::wind_create(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t kind		= ntohs(p[0]);
	int16_t x			= ntohs(p[1]);
	int16_t y			= ntohs(p[2]);
	int16_t w			= ntohs(p[3]);
	int16_t h			= ntohs(p[4]);

	int16_t physId = wind_create(ws->handle(), kind, x, y, w, h);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(physId);
	cm->setType(MSG_REPLY(ClientMsg::AES_WIND_CREATE));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

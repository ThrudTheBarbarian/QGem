
#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* 6604: Create and register a window
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
			.kind	= kind,
			.x		= x,
			.y		= y,
			.w		= w,
			.h		= h,
			.shown	= false
			};
		_windowList.push_back(win);
		winId = (int) _windowList.size();
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
	cm->append(physId);
	cm->setType(MSG_REPLY(ClientMsg::AES_WIND_CREATE));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

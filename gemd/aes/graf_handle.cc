
#include "aes.h"
#include "clientmsg.h"
#include "connectionmgr.h"
#include "fontmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* 6902: Retrieve the AES physical workstation id and char stats.
|*
|* returns -1 or the workstation id
\*****************************************************************************/
int16_t AES::graf_handle(qintptr handle, int16_t *info)
	{
	int16_t physId = -1;

	ConnectionMgr *cm = _vdi->screen()->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		/*********************************************************************\
		|* Fetch the metrics
		\*********************************************************************/
		FontMgr::sharedInstance().boxMetrics(FontMgr::SYSTEM_FONT_ID,
											 ws->textEffect(),
											 ws->textHeight(),
											 info[0], info[1],
											 info[2], info[3]);
		physId = 0;	// Physical id is always 0
		}

	return physId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::graf_handle(Workstation *ws, ClientMsg *cm)
	{
	int16_t info[4];
	int16_t physId = graf_handle(ws->handle(), info);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(physId);
	cm->append(info, 4);
	cm->setType(MSG_REPLY(ClientMsg::AES_GRAF_HANDLE));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

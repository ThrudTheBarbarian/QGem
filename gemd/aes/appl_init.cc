
#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"


/*****************************************************************************\
|* 6007: Register the application with the AES and get an app-id.
|*
|* returns -1 or the application id
\*****************************************************************************/
int16_t AES::appl_init(qintptr handle, QString uuid)
	{
	AppContext ctx =
		{
		.appId		= 0,
		.uuid		= "",
		.cmd		= "",
		.args		= "",
		.isDesktop	= false,
		.handle		= 0
		};

	/**************************************************************************\
	|* If we have a uuid, use that app-context
	\**************************************************************************/
	int idx = 0;
	for (auto i = _pendingApps->begin(); i != _pendingApps->end(); i++)
		{
		if (i->uuid == uuid)
			{
			ctx = _pendingApps->takeAt(idx);
			break;
			}
		idx ++;
		}

	ctx.appId		= _nextApp ++;
	_apps[handle]	= ctx;
	return ctx.appId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::appl_init(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Parse out the UUID for the application, if one was sent
	\**************************************************************************/
	QString appUuid = "";
	if (cm->payload().size() > 0)
		{
		QByteArray ba;
		cm->fetchData(0, ba);
		appUuid = QString::fromLatin1(ba.data());
		}

	int16_t appId = appl_init(ws->handle(), appUuid);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(appId);
	cm->setType(MSG_REPLY(ClientMsg::AES_APPL_INIT));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

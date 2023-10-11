
#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"


/*****************************************************************************\
|* 6007: Register the application with the AES and get an app-id.
|*
|* returns -1 or the application id
\*****************************************************************************/
int16_t AES::appl_init(qintptr handle)
	{
	AppContext ctx		=
		{
		.appId			= _nextApp ++
		};

	_apps[handle]		= ctx;
	return ctx.appId;
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void AES::appl_init(Workstation *ws, ClientMsg *cm)
	{
	int16_t appId = appl_init(ws->handle());

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

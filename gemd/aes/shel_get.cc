#include <QDir>
#include <QFileInfo>

#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"

/******************************************************************************\
|* 7902: Return the desktop environment
\******************************************************************************/
void AES::shel_get(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Send the data down the wire
	\**************************************************************************/
	cm->clear();
	QByteArray ba		= _deskEnv.toLatin1();
	const char *string	= ba.data();
	cm->append((uint8_t *)string, ba.length()+1);

	cm->setType(MSG_REPLY(ClientMsg::AES_SHEL_GET));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

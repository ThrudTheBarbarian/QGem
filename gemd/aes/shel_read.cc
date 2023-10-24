#include <QDir>
#include <QFileInfo>

#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"


void AES::shel_read(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Send the data down the wire
	\**************************************************************************/
	cm->clear();

	QString cmd		= "";
	QString args	= "";

	/*************************************************************************\
	|* Get the context for this connection
	\*************************************************************************/
	qintptr handle = ws->handle();
	if (_apps.contains(handle))
		{
		AppContext& ctx = _apps[handle];
		cmd				= ctx.cmd;
		args			= ctx.args;
		}

	QByteArray cmdBA	= cmd.toLatin1();
	cm->append((uint8_t *)cmdBA.data(), cmdBA.length()+1);

	QByteArray argsBA	= args.toLatin1();
	cm->append((uint8_t *)argsBA.data(), argsBA.length()+1);

	cm->setType(MSG_REPLY(ClientMsg::AES_SHEL_READ));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

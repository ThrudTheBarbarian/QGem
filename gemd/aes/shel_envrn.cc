
#include "aes.h"
#include "clientmsg.h"
#include "workstation.h"

/*****************************************************************************\
|* 7900: Get an environment variable from the shell.
|*
|* Returns the environment variable or NULL
\*****************************************************************************/
void AES::shel_envrn(Workstation *ws, ClientMsg *cm)
	{
	QByteArray bytes;
	cm->fetchData(0, bytes);

	std::string value;
	std::string name = bytes.toStdString();
	bool found = ws->findEnvironmentVar(name, value);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(found);
	if (found)
		cm->append((uint8_t *)value.c_str(), value.length()+1);

	cm->setType(MSG_REPLY(ClientMsg::AES_SHEL_ENVRN));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

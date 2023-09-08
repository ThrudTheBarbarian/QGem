
#include "clientmsg.h"
#include "debug.h"
#include "socketthread.h"
#include "vdi.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
VDI::VDI(QObject *parent)
	:QObject{parent}
	,_io(nullptr)
	{}


#pragma mark - Private Methods

/*****************************************************************************\
|* Connect to the server
\*****************************************************************************/
bool VDI::_connectToGemDaemon(void)
	{
	if (_io == nullptr)
		{
		_io = new SocketThread();
		_io->run();
		}

	return _io->connected();;
	}


/*****************************************************************************\
|* Send a blocking message, ie: send a message and wait for a reply
\*****************************************************************************/
bool VDI::_sendBlockingMessage(ClientMsg& msg)
	{
	bool ok = false;
	if (_io != nullptr)
		{
		ok = _io->send(msg, true);
		}
	else
		WARN("Attempt to set msg [type=%d] without connection", msg.type());

	return ok;
	}


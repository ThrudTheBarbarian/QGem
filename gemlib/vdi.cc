
#include "constants.h"
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
VDI::VDI(QObject *parent)
	:QObject{parent}
	,_connected(false)
	{}


#pragma mark - Private Methods

/*****************************************************************************\
|* Connect to the server
\*****************************************************************************/
bool VDI::_connectToGemDaemon(void)
	{
	bool ok = false;

	_sock.connectToServer(SOCKET_NAME);
	if (_sock.isValid())
		{
		ok = true;
		}
	else
		{
		WARN("Cannot bind to socket %s", SOCKET_NAME);
		}

	return ok;
	}

#pragma mark - Private Slots

/*****************************************************************************\
|* Handle incoming async data
\*****************************************************************************/
void VDI::_socketDataReady(void)
	{
	if (_sock.bytesAvailable() > 2)
		{

		}
	}

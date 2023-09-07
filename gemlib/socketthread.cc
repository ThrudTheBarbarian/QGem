#include "constants.h"
#include "debug.h"
#include "socketthread.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
SocketThread::SocketThread()
			 :QThread()
			 ,_connected(false)
	{}

/*****************************************************************************\
|* Override run to set things up then call exec() to start up the event loop
\*****************************************************************************/
void SocketThread::run(void)
	{
	_sock.connectToServer(SOCKET_NAME);
	if (!_sock.isValid())
		{
		WARN("Cannot bind to socket %s", SOCKET_NAME);
		}

	/*************************************************************************\
	|* Wire up the socket to send notifications if it has received data
	\*************************************************************************/
	connect(&_sock, &QLocalSocket::readyRead,
			this, QOverload<>::of(&SocketThread::_socketDataReady));

	/*************************************************************************\
	|* Start the event loop
	\*************************************************************************/
	exec();
	}


/*****************************************************************************\
|* Write to the socket
\*****************************************************************************/
bool SocketThread::send(QByteArray& ba)
	{
	std::lock_guard<std::recursive_mutex> lock(_mutex);
	bool ok = false;

	if (_sock.write(ba) == ba.size())
		ok = true;

	return ok;
	}

#pragma mark - Private Slots

/*****************************************************************************\
|* Handle incoming async data
\*****************************************************************************/
void SocketThread::_socketDataReady(void)
	{
	if (_sock.bytesAvailable() > 2)
		{

		}
	}

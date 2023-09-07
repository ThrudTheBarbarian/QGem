#include "constants.h"
#include "debug.h"
#include "socketthread.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
SocketThread::SocketThread()
			 :QThread()
			 ,_connected(false)
			 ,_sock(nullptr)
			 ,_isBlocking(false)
	{
	}

/*****************************************************************************\
|* Override run to set things up then call exec() to start up the event loop
|*
|* Note that signals will only be answere on this thread for entities created
|* in the run method below.
\*****************************************************************************/
void SocketThread::run(void)
	{
	_sock = new QLocalSocket();
	_sock->connectToServer(SOCKET_NAME);
	if (!_sock->isValid())
		{
		WARN("Cannot bind to socket %s", SOCKET_NAME);
		}

	/*************************************************************************\
	|* Wire up the socket to send notifications if it has received data
	\*************************************************************************/
	connect(_sock, &QLocalSocket::readyRead,
			this, QOverload<>::of(&SocketThread::_socketDataReady));

	/*************************************************************************\
	|* Start the event loop
	\*************************************************************************/
	exec();
	}


/*****************************************************************************\
|* Write to the socket
\*****************************************************************************/
bool SocketThread::send(ClientMsg& cm, bool isBlocking)
	{
	std::lock_guard<std::recursive_mutex> lock(_mutex);
	bool ok = false;

	if (isBlocking)
		{
		/*********************************************************************\
		|* Bump the number of semaphore levels, and immediately acquire it
		\*********************************************************************/
		_await.release();
		_await.acquire();

		/*********************************************************************\
		|* Set blocking mode and the type of message we're looking for
		\*********************************************************************/
		_isBlocking		= isBlocking;
		_blockedTypes.push_back(-cm.type());
		}

	QByteArray ba = cm.encode();
	if (_sock->write(ba) == ba.size())
		ok = true;

	return ok;
	}

#pragma mark - Private Methods

/*****************************************************************************\
|* Dispatch the message to preserved state
\*****************************************************************************/
void SocketThread::_dispatch(ClientMsg& msg)
	{
	fprintf(stderr, "Got message of type 0x%04X", msg.type());
	}

#pragma mark - Private Slots

/*****************************************************************************\
|* Handle incoming async data
\*****************************************************************************/
void SocketThread::_socketDataReady(void)
	{
	ClientMsg cm;
	if (cm.read(_sock))
		{
		if (_isBlocking)
			{
			if (cm.type() == _blockedTypes.back())
				{
				_dispatch(cm);

				_blockedTypes.pop_back();
				_isBlocking = (_blockedTypes.size() > 0);

				_await.release();

				for (ClientMsg& msg : _pending)
					_dispatch(msg);
				}
			else
				_pending.push_back(cm);
			}
		else
			_dispatch(cm);
		}
	}

#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QLocalSocket>
#include <QObject>
#include <QSemaphore>
#include <QThread>

#include <mutex>

#include "clientmsg.h"
#include "properties.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class SocketThread : public QThread
	{
	Q_OBJECT

		/*********************************************************************\
		|* typedefs and defines etc
		\*********************************************************************/
		typedef std::vector<int16_t> MessageTypeList;

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GET(bool, connected);           // Are we connected to the server ?
		GET(QSemaphore, await);			// Actual blocking mechanims for sync


	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		std::recursive_mutex _mutex;	// Only 1 thread can send at once
		QLocalSocket * _sock;			// Socket for communication

		QVector<ClientMsg> _pending;	// incoming msgs while in blocking mode
		bool _isBlocking;				// Are we in blocking mode
		MessageTypeList _blockedTypes;	// What to block for

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit SocketThread(void);

		/*********************************************************************\
		|* Override run() to set things up and then call exec()
		\*********************************************************************/
		void run(void) override;

		/*********************************************************************\
		|* send data down the socket
		\*********************************************************************/
		bool send(ClientMsg &cm, bool isBlocking=false);

		private:
		/*********************************************************************\
		|* Private method: handle dispatching the incoming message
		\*********************************************************************/
		void _dispatch(ClientMsg& msg);

		private slots:
		/*********************************************************************\
		|* Private slot: there is data on the incoming socket, read, parse,
		|* and process
		\*********************************************************************/
		void _socketDataReady(void);
	};

#endif // SOCKETTHREAD_H

#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QLocalSocket>
#include <QObject>
#include <QThread>

#include <mutex>

#include "properties.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class SocketThread : public QThread
	{
	Q_OBJECT

		/*********************************************************************\
		|* Properties
		\*********************************************************************/
		GET(bool, connected);           // Are we connected to the server ?
		GET(QLocalSocket, sock);		// Socket to display-server process


	private:
		/*************************************************************************\
		|* Private state : mutex for writing to the socket
		\*************************************************************************/
		std::recursive_mutex _mutex;

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
		bool send(QByteArray &ba);

	private slots:
		/*********************************************************************\
		|* Private slot: there is data on the incoming socket, read, parse,
		|* and process
		\*********************************************************************/
		void _socketDataReady(void);
	};

#endif // SOCKETTHREAD_H

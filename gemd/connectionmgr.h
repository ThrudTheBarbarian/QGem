#ifndef CONNECTIONMGR_H
#define CONNECTIONMGR_H

#include <QMap>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>


/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ConnectionMgr : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:
		QMap<qintptr, QLocalSocket*> _conns;	// List of connected clients
		QLocalServer _server;					// Server socket to listen on

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit ConnectionMgr(QObject *parent = nullptr);

		/*********************************************************************\
		|* start listening
		\*********************************************************************/
		void start(void);

		/*********************************************************************\
		|* stop listening
		\*********************************************************************/
		void stop(void);

	signals:

	private slots:
		/*********************************************************************\
		|* Slot: We got a new connection
		\*********************************************************************/
		void _connection(void);

		/*********************************************************************\
		|* Slot: We lost a connection
		\*********************************************************************/
		void _disconnection(void);

		/*********************************************************************\
		|* Slot: A client has data to read
		\*********************************************************************/
		void _incomingData(void);

	};

#endif // CONNECTIONMGR_H

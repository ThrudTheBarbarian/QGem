#ifndef CONNECTIONMGR_H
#define CONNECTIONMGR_H

#include <QList>
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
		QList<QLocalSocket*> _conns;	// List of connected applications
		QLocalServer _server;			// Server socket to listen on

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

	public slots:
		/*********************************************************************\
		|* Slot: We got a new connection
		\*********************************************************************/
		void connection();

		/*********************************************************************\
		|* Slot: We lost a connection
		\*********************************************************************/
		void disconnection();

	};

#endif // CONNECTIONMGR_H

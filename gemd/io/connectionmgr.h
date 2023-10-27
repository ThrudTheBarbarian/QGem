#ifndef CONNECTIONMGR_H
#define CONNECTIONMGR_H

#include <QMap>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>

#include "macros.h"
#include "properties.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class Screen;
class Workstation;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ConnectionMgr : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Typedefs, consts etc
	\*************************************************************************/
	const char *SOCKET_IDENTIFIER = "socket_identifier";

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GET(HandleList, timerList);					// List of handles that have
												// timer-events registered

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:
		QMap<qintptr, Workstation*> _conns;		// List of connected clients
		QLocalServer _server;					// Server socket to listen on
		Screen * _screen;						// Screen to draw on

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit ConnectionMgr(QObject *parent = nullptr);

		/*********************************************************************\
		|* start listening
		\*********************************************************************/
		void start(Screen *screen);

		/*********************************************************************\
		|* stop listening
		\*********************************************************************/
		void stop(void);

		/*********************************************************************\
		|* Return the workstation for a given handle
		\*********************************************************************/
		Workstation * findWorkstationForHandle(qintptr handle);

		/*********************************************************************\
		|* Add or remove a connection from the list that wants timer events
		\*********************************************************************/
		void updateTimerInterest(qintptr handle, int16_t enable);

		/*********************************************************************\
		|* Set the physical workstation as handle 0
		\*********************************************************************/
		void setPhysicalWorkstation(Workstation *ws);

	signals:
		/*********************************************************************\
		|* Signal: we connected this virtual workstation
		\*********************************************************************/
		void connection(Workstation *ws);

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

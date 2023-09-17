#ifndef SCREEN_H
#define SCREEN_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Screen; }
QT_END_NAMESPACE

#include "properties.h"

/*****************************************************************************\
|* Forward references
\*****************************************************************************/
class ConnectionMgr;
class Workstation;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class Screen : public QMainWindow
	{
	Q_OBJECT

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:
		QTimer			_cron;					// Used for framerate control
		Ui::Screen *	_ui;					// User interface definition
		ConnectionMgr *	_conmgr;				// Connection manager

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GET(bool, wasMaximised);					// Were we maximised before ?

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		Screen(QWidget *parent = nullptr);
		~Screen();

		/*********************************************************************\
		|* Return the connection manager for this screen
		\*********************************************************************/
		inline ConnectionMgr * connectionManager(void)
			{ return _conmgr; }


		/*********************************************************************\
		|* Install an event-filter to get mouse-movement
		\*********************************************************************/
		bool eventFilter(QObject *obj, QEvent *event);

	public slots:
		/*********************************************************************\
		|* Slot: update the main widget
		\*********************************************************************/
		void doFrameUpdate(void);

		/*********************************************************************\
		|* Slot: We got a connection
		\*********************************************************************/
		void connectedTo(Workstation *ws);


	private:
	};

#endif // SCREEN_H

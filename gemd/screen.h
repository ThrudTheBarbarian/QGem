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

	public slots:
		/*********************************************************************\
		|* Slot: update the main widget
		\*********************************************************************/
		void doFrameUpdate(void);


	private:
	};

#endif // SCREEN_H

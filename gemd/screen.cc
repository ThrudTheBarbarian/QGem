#include <QMouseEvent>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "ui_screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Class constructor
\*****************************************************************************/
Screen::Screen(QWidget *parent)
	   :QMainWindow(parent)
	   ,_ui(new Ui::Screen)
	{
	/*************************************************************************\
	|* Create the UI
	\*************************************************************************/
	_ui->setupUi(this);
	qApp->installEventFilter(this);

	/*************************************************************************\
	|* Start the frame-update timer going
	\*************************************************************************/
	connect(&_cron, &QTimer::timeout,
			this, QOverload<>::of(&Screen::doFrameUpdate));
	_cron.start(10);

	/*************************************************************************\
	|* Start the connection manager to listen for clients
	\*************************************************************************/
	_conmgr = new ConnectionMgr();
	_conmgr->start(this);

	/*************************************************************************\
	|* We want to know when the connection manager got a hit
	\*************************************************************************/
	connect(_conmgr, &ConnectionMgr::connection, this, &Screen::connectedTo);
	}

/*****************************************************************************\
|* Class destructor
\*****************************************************************************/
Screen::~Screen()
	{
	delete _ui;
	}


/*****************************************************************************\
|* Update the frame
\*****************************************************************************/
void Screen::doFrameUpdate(void)
	{
	_ui->centralwidget->update();
	}

/*****************************************************************************\
|* Make sure we get mouse-move events
\*****************************************************************************/
bool Screen::eventFilter(QObject *, QEvent *event)
	{
	static QPointF old = {-1,-1};

	if (event->type() == QEvent::MouseMove)
		{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		QPointF pos = mouseEvent->pos();
		if (pos != old)
			{
			_propagateEvent(mouseEvent,
							VDI::MouseMove,
							ClientMsg::EVT_MOUSE_MOVE);
			old = pos;
			}
		}
	return false;
	}


/*****************************************************************************\
|* Create and send a mouse event if necessary
\*****************************************************************************/
void Screen::_propagateEvent(QEvent *e, int filter, int16_t type)
	{
	VDI& vdi = VDI::sharedInstance();
	Workstation *ws = vdi.top();

	if (ws != nullptr)
		{
		if (vdi.activeEvents() & filter)
			{
			ClientMsg msg;
			if (type == ClientMsg::EVT_KEY_PRESS)
				msg.fromKeyEvent(static_cast<QKeyEvent*>(e), type);
			else
				msg.fromMouseEvent(static_cast<QMouseEvent*>(e), type);
			ws->client()->write(msg.encode());
			}
		}
	}

/*****************************************************************************\
|* Handle mouse-down events
\*****************************************************************************/
void Screen::mousePressEvent(QMouseEvent *event)
	{
	_propagateEvent(event, VDI::MouseButton, ClientMsg::EVT_MOUSE_DOWN);
	}

/*****************************************************************************\
|* Handle mouse-release events
\*****************************************************************************/
void Screen::mouseReleaseEvent(QMouseEvent *event)
	{
	_propagateEvent(event, VDI::MouseButton, ClientMsg::EVT_MOUSE_UP);
	}

/*****************************************************************************\
|* Handle key-press events
\*****************************************************************************/
void Screen::keyPressEvent(QKeyEvent *event)
	{
	_propagateEvent(event, VDI::Keyboard, ClientMsg::EVT_KEY_PRESS);
	}

/*****************************************************************************\
|* We got a connection, grab focus
\*****************************************************************************/
void Screen::connectedTo(Workstation *)
	{
	raise();			// Seems to be needed on the Mac
	activateWindow();	// Should be all that's necessary
	}

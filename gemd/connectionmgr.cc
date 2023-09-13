#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

#define SERVICE_NAME "/tmp/gemd"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ConnectionMgr::ConnectionMgr(QObject *parent)
			  :QObject{parent}
	{
	_server.setSocketOptions(QLocalServer::WorldAccessOption);

	/*************************************************************************\
	|* Clean up any remnants from a previous run
	\*************************************************************************/
	QLocalServer::removeServer(SERVICE_NAME);

	/*************************************************************************\
	|* Connect up the QLocalServer so it handles incoming connections
	\*************************************************************************/
	connect(&_server, &QLocalServer::newConnection,
			this, QOverload<>::of(&ConnectionMgr::_connection));

	}

/*****************************************************************************\
|* Start listening
\*****************************************************************************/
void ConnectionMgr::start(Screen *screen)
	{
	_screen = screen;
	_server.listen(SERVICE_NAME);

	const std::string& stdS = _server.fullServerName().toStdString();
	fprintf(stderr, "Now listening on %s\n", stdS.c_str());
	}

/*****************************************************************************\
|* Stop listening
\*****************************************************************************/
void ConnectionMgr::stop(void)
	{
	_server.close();
	}

/*****************************************************************************\
|* We got a new connection
\*****************************************************************************/
void ConnectionMgr::_connection(void)
	{
	QLocalSocket *client = _server.nextPendingConnection();
	while (client != nullptr)
		{
		Workstation *ws = new Workstation(client, this);
		_conns[client->socketDescriptor()] = ws;

		connect(client, &QLocalSocket::disconnected,
				this, QOverload<>::of(&ConnectionMgr::_disconnection));

		connect(client, &QLocalSocket::readyRead,
				this, QOverload<>::of(&ConnectionMgr::_incomingData));

		client = _server.nextPendingConnection();
		}
	fprintf(stderr, "Connection! %d clients\n", (int) _conns.size());
	}

/*****************************************************************************\
|* We got a disconnection
\*****************************************************************************/
void ConnectionMgr::_disconnection(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();
	_conns.remove(socket->socketDescriptor());

	fprintf(stderr, "Disconnection! %d left\n", (int) _conns.size());
	}

/*****************************************************************************\
|* Return the workstation for a given handle
\*****************************************************************************/
Workstation * ConnectionMgr::findWorkstationForHandle(qintptr handle)
	{
	if (_conns.contains(handle))
		return _conns[handle];
	return nullptr;
	}


/*****************************************************************************\
|* Set the physical workstation as handle 0
\*****************************************************************************/
void ConnectionMgr::setPhysicalWorkstation(Workstation *ws)
	{
	_conns[0] = ws;
	}

/*****************************************************************************\
|* We got incoming...
\*****************************************************************************/
void ConnectionMgr::_incomingData(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();
	Workstation *ws			= _conns[socket->socketDescriptor()];

	ClientMsg cm;
	while (cm.read(socket))
		{
		fprintf(stderr, "Despatch message of type: %d\n", cm.type());
		switch (cm.type())
			{
			case ClientMsg::V_CLRWK:		// 3
				VDI::sharedInstance().v_clrwk(ws);
				break;

			case ClientMsg::VQ_CHCELLS:		// 5.1
				VDI::sharedInstance().vq_chcells(ws, &cm);
				break;

			case ClientMsg::VQ_EXIT_CUR:	// 5.3
				VDI::sharedInstance().vq_exit_cur(ws);
				break;

			case ClientMsg::V_ENTER_CUR:	// 5.3
				VDI::sharedInstance().v_enter_cur(ws);
				break;

			case ClientMsg::V_CURUP:		// 5.4
				VDI::sharedInstance().v_curup(ws);
				break;

			case ClientMsg::V_CURDOWN:		// 5.5
				VDI::sharedInstance().v_curdown(ws);
				break;

			case ClientMsg::V_CURRIGHT:		// 5.6
				VDI::sharedInstance().v_curright(ws);
				break;

			case ClientMsg::V_CURLEFT:		// 5.7
				VDI::sharedInstance().v_curleft(ws);
				break;

			case ClientMsg::V_CURHOME:		// 5.8
				VDI::sharedInstance().v_curhome(ws);
				break;

			case ClientMsg::V_EEOS:			// 5.9
				VDI::sharedInstance().v_eeos(ws);
				break;

			case ClientMsg::V_EEOL:			// 5.10
				VDI::sharedInstance().v_eeol(ws);
				break;

			case ClientMsg::VS_CURADDRESS:	// 5.11
				VDI::sharedInstance().vs_curaddress(ws, &cm);
				break;

			case ClientMsg::V_CURTEXT:		// 5.12
				VDI::sharedInstance().v_curtext(ws, &cm);
				break;

			case ClientMsg::V_RVON	:		// 5.13
				VDI::sharedInstance().v_rvon(ws);
				break;

			case ClientMsg::V_RVOFF:		// 5.14
				VDI::sharedInstance().v_rvoff(ws);
				break;

			case ClientMsg::VQ_CURADDRESS:	// 5.15
				VDI::sharedInstance().vq_curaddress(ws, &cm);
				break;

			case ClientMsg::V_DSPCUR:		// 5.18
				VDI::sharedInstance().v_dspcur(ws, &cm);
				break;

			case ClientMsg::V_RMCUR:		// 5.19
				VDI::sharedInstance().v_rmcur(ws);
				break;

			case ClientMsg::V_PLINE:		// 6
				VDI::sharedInstance().v_pline(ws, &cm);
				break;

			case ClientMsg::V_PMARKER:		// 6
				VDI::sharedInstance().v_pmarker(ws, &cm);
				break;

			case ClientMsg::VSL_TYPE:		// 15
				VDI::sharedInstance().vsl_type(ws, &cm);
				break;

			case ClientMsg::VSL_WIDTH:		// 16
				VDI::sharedInstance().vsl_width(ws, &cm);
				break;

			case ClientMsg::VSL_COLOR:		// 17
				VDI::sharedInstance().vsl_color(ws, &cm);
				break;

			case ClientMsg::VSM_TYPE:		// 18
				VDI::sharedInstance().vsm_type(ws, &cm);
				break;

			case ClientMsg::VSM_HEIGHT:		// 19
				VDI::sharedInstance().vsm_height(ws, &cm);
				break;

			case ClientMsg::VSM_COLOR:		// 20
				VDI::sharedInstance().vsm_color(ws, &cm);
				break;

			case ClientMsg::V_OPNVWK:		// 100
				VDI::sharedInstance().v_opnvwk(ws, &cm);
				break;

			case ClientMsg::VSL_ENDS:		// 108
				VDI::sharedInstance().vsl_ends(ws, &cm);
				break;

			case ClientMsg::VS_CLIP:		// 129
				VDI::sharedInstance().vs_clip(ws, &cm);
				break;

			default:
				WARN("Unknown message type %d", cm.type());
				break;
			}
		}
	}

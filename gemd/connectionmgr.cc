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
		Workstation *ws = new Workstation(client, _screen, this);
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
|* We got incoming...
\*****************************************************************************/
void ConnectionMgr::_incomingData(void)
	{
	QLocalSocket *socket	= (QLocalSocket *) QObject::sender();
	Workstation *ws			= _conns[socket->socketDescriptor()];

	ClientMsg cm;
	while (cm.read(socket))
		{
		switch (cm.type())
			{
			case ClientMsg::V_OPNVWK:
				{
				return VDI::sharedInstance().v_opnvwk(ws, &cm);
				}

			default:
				WARN("Unknown message type %d", cm.type());
				break;
			}
		}
	}

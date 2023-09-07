#include "clientmsg.h"
#include "connectionmgr.h"

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
			this, QOverload<>::of(&ConnectionMgr::connection));

	}

/*****************************************************************************\
|* Start listening
\*****************************************************************************/
void ConnectionMgr::start(void)
	{
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
void ConnectionMgr::connection(void)
	{
	QLocalSocket *client = _server.nextPendingConnection();
	while (client != nullptr)
		{
		_conns.push_back(client);
		connect(client, &QLocalSocket::disconnected,
				this, QOverload<>::of(&ConnectionMgr::disconnection));

		client = _server.nextPendingConnection();
		}
	fprintf(stderr, "Connection! %d clients\n", (int) _conns.size());
	}

/*****************************************************************************\
|* We got a disconnection
\*****************************************************************************/
void ConnectionMgr::disconnection(void)
	{
	erase(_conns, QObject::sender());
	fprintf(stderr, "Disconnection! %d left\n", (int) _conns.size());
	}

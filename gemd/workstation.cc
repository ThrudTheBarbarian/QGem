#include "clientmsg.h"
#include "gem.h"
#include "macros.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Class definition for virtual workstation
\*****************************************************************************/
Workstation::Workstation(QLocalSocket *client, QObject *parent)
			:QObject{parent}
			,_deviceDriver(1)
			,_lineType(SOLID)
			,_lineColour(G_BLACK)
			,_markerType(MRKR_DOT)
			,_markerColour(G_BLACK)
			,_fontId(0)
			,_textColour(G_BLACK)
			,_fillType(FIS_SOLID)
			,_fillStyle(PT_DOTS1)
			,_fillColour(G_BLUE)
			,_coordType(COORD_RASTER)
			,_pageSize(PG_DIN_A4)
			,_client(client)
	{
	setDefaultColours();
	}


/*****************************************************************************\
|* Class definition for physical workstation
\*****************************************************************************/
Workstation::Workstation(QObject *parent)
	:QObject{parent}
	,_deviceDriver(1)
	,_lineType(SOLID)
	,_lineColour(G_BLACK)
	,_markerType(MRKR_DOT)
	,_markerColour(G_BLACK)
	,_fontId(0)
	,_textColour(G_BLACK)
	,_fillType(FIS_SOLID)
	,_fillStyle(PT_DOTS1)
	,_fillColour(G_BLUE)
	,_coordType(COORD_RASTER)
	,_pageSize(PG_DIN_A4)
	,_client(nullptr)
	{
	setDefaultColours();
	}


/*****************************************************************************\
|* Destructor
\*****************************************************************************/
Workstation::~Workstation(void)
	{
	}


/*****************************************************************************\
|* Write to the client
\*****************************************************************************/
void Workstation::send(ClientMsg* msg, bool log)
	{
	QByteArray ba = msg->encode();
	if (log)
		fprintf(stderr, "Writing %d bytes to socket handle %d\n",
				(int)ba.size(),
				(int)(_client->socketDescriptor()));

	_client->write(ba);
	}

/*****************************************************************************\
|* Set the default colours for the workstation
\*****************************************************************************/
void Workstation::setDefaultColours(void)
	{
	setColour(0,  255, 255, 255);		// white
	setColour(1,  0,   0,   0);			// black
	setColour(2,  255, 0,   0);			// red
	setColour(3,  0,   255, 0);			// green
	setColour(4,  0,   0,   255);		// blue
	setColour(5,  0,   255, 255);		// cyan
	setColour(6,  255, 255, 0);			// yellow
	setColour(7,  255, 0,   255);		// magenta
	setColour(8,  182, 182, 182);		// grey
	setColour(9,  128, 128, 128);		// dark grey
	setColour(10, 255, 109, 109);		// light red
	setColour(11, 109, 109, 255);		// light green
	setColour(12, 109, 255, 109);		// light blue
	setColour(13, 109, 255, 255);		// light cyan
	setColour(14, 255, 255, 109);		// light yellow
	setColour(15, 255, 109, 255);		// light magenta
	}

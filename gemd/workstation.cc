#include "clientmsg.h"
#include "gem.h"
#include "macros.h"
#include "vdi.h"
#include "workstation.h"


static Qt::PenStyle _styles[] =
	{
		Qt::SolidLine,			// 0 is undefined
		Qt::SolidLine,			// SOLID
		Qt::DashLine,			// LDASHED
		Qt::DotLine,			// DOTTED
		Qt::DashDotLine,		// DASHDOT
		Qt::DashLine,			// DASH
		Qt::DashDotDotLine,		// DASHDOTDOT
		Qt::CustomDashLine		// USERLINE
};

/*****************************************************************************\
|* Class definition for virtual workstation
\*****************************************************************************/
Workstation::Workstation(QLocalSocket *client, QObject *parent)
			:QObject{parent}
			,_deviceDriver(1)
			,_lineType(SOLID)
			,_lineColour(G_BLACK)
			,_lineWidth(1)
			,_markerType(MRKR_CIRCLE)
			,_markerColour(G_BLACK)
			,_markerHeight(5)
			,_fontId(0)
			,_textColour(G_BLACK)
			,_fillType(FIS_SOLID)
			,_fillStyle(PT_DOTS1)
			,_fillColour(G_BLUE)
			,_coordType(COORD_RASTER)
			,_pageSize(PG_DIN_A4)
			,_wrMode(QPainter::CompositionMode_Source)
			,_clip(QRect(0,0,0,0))
			,_enableClip(false)
			,_startCap(CAP_SQUARE)
			,_endCap(CAP_SQUARE)
			,_client(client)
	{
	setDefaultColours();
	_userType << 3 << 1;
	}


/*****************************************************************************\
|* Class definition for physical workstation
\*****************************************************************************/
Workstation::Workstation(QObject *parent)
	:QObject{parent}
	,_deviceDriver(1)
	,_lineType(SOLID)
	,_lineColour(G_BLACK)
	,_lineWidth(1)
	,_markerType(MRKR_CIRCLE)
	,_markerColour(G_BLACK)
	,_markerHeight(5)
	,_fontId(0)
	,_textColour(G_BLACK)
	,_fillType(FIS_SOLID)
	,_fillStyle(PT_DOTS1)
	,_fillColour(G_BLUE)
	,_coordType(COORD_RASTER)
	,_pageSize(PG_DIN_A4)
	,_wrMode(QPainter::CompositionMode_Source)
	,_clip(QRect(0,0,0,0))
	,_enableClip(false)
	,_startCap(CAP_SQUARE)
	,_endCap(CAP_SQUARE)
	,_client(nullptr)
	{
	setDefaultColours();
	_userType << 3 << 1;
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

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForLine(QPen& pen)
	{
	if (_lineType == DASH)
		{
		QList<qreal> dashes;
		dashes << 3 << 3;
		pen.setDashPattern(dashes);
		}
	else if (_lineType == USERLINE)
		pen.setDashPattern(_userType);
	else
		pen.setStyle(_styles[_lineType]);

	pen.setColor(_palette[_lineColour]);
	pen.setBrush(_palette[_lineColour]);
	pen.setWidth(_lineWidth);

	}

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForMarker(QPen& pen)
	{
	pen.setStyle(_styles[SOLID]);

	pen.setColor(_palette[_markerColour]);
	pen.setBrush(_palette[_markerColour]);
	pen.setWidth(1);
	}

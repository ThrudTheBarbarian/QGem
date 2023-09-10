#include "gem.h"
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
	{}


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
	{}

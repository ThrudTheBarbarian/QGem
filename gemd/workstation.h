#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <QLocalSocket>
#include <QObject>
#include <QPainter>

#include "properties.h"

/*****************************************************************************\
|* Forward references
\*****************************************************************************/
class Screen;

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class Workstation : public QObject
	{
	Q_OBJECT

	/**************************************************************************\
	|* Properties
	\**************************************************************************/
	GETSET(int, deviceDriver, DeviceDriver);			// 1=this, 2+n = switch
	GETSET(int, lineType, LineType);					// SOLID -> USERLINE
	GETSET(int, lineColour, LineColour);				// colour to draw
	GETSET(int, markerType, MarkerType);				// MRKR_{DOT -> DIAMOND}
	GETSET(int, markerColour, MarkerColour);			// colour for markers
	GETSET(int, fontId, fontId);						// font id to use
	GETSET(int, textColour, TextColour);				// colour index for text
	GETSET(int, fillType, FillType);					// type of fill to use
	GETSET(int, fillStyle, FillStyle);					// style of fill to use
	GETSET(int, fillColour, FillColour);				// colour index for fill
	GETSET(int, coordType, CoordType);					// NDC or Raster co-ords
	GETSET(int, pageSize, PageSize);					// Page size to use
	GETSET(QPainter::CompositionMode, wrMode, WrMode);	// Composition mode

	private:
		/*********************************************************************\
		|* Socket to communicate over
		\*********************************************************************/
		QLocalSocket *_client;					// Connection to client

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit Workstation(QObject *parent = nullptr);
		explicit Workstation(QLocalSocket *client,
							 QObject *parent = nullptr);
		~Workstation(void);

		/*********************************************************************\
		|* Return the socket
		\*********************************************************************/
		inline QLocalSocket * client(void) { return _client; }

	signals:


	};

#endif // WORKSTATION_H

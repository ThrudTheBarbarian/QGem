#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <QLocalSocket>
#include <QObject>

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
	GETSET(int, deviceDriver, DeviceDriver);	// 1=current res, 2+n = switch
	GETSET(int, lineType, LineType);			// SOLID -> USERLINE
	GETSET(int, lineColour, LineColour);		// colour index for drawing in
	GETSET(int, markerType, MarkerType);		// MRKR_DOT -> MRKR_DIAMOND
	GETSET(int, markerColour, MarkerColour);	// colour index for markers
	GETSET(int, fontId, fontId);				// font id to use
	GETSET(int, textColour, TextColour);		// colour index for text
	GETSET(int, fillType, FillType);			// type of fill to use
	GETSET(int, fillStyle, FillStyle);			// style of fill to use
	GETSET(int, fillColour, FillColour);		// colour index for fill
	GETSET(int, coordType, CoordType);			// NDC or Raster co-ords
	GETSET(int, pageSize, PageSize);			// Which page size to use

	private:
		/*********************************************************************\
		|* Socket to communicate over
		\*********************************************************************/
		QLocalSocket *_client;					// Connection to client
		Screen * _screen;						// Screen to draw on

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit Workstation(QLocalSocket *client,
							 Screen *screen,
							 QObject *parent = nullptr);

		/*********************************************************************\
		|* Return the socket
		\*********************************************************************/
		QLocalSocket * client(void) { return _client; }

		/*********************************************************************\
		|* Return the screen
		\*********************************************************************/
		Screen * screen(void) { return _screen; }

	signals:


	};

#endif // WORKSTATION_H

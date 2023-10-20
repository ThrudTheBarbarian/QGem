#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <QLocalSocket>
#include <QObject>
#include <QPainter>

#include "properties.h"
#include "macros.h"

/*****************************************************************************\
|* Forward references
\*****************************************************************************/
class ClientMsg;
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
	GET(QFont, currentFont);							// Current font
	GETSET(int, deviceDriver, DeviceDriver);			// 1=this, 2+n = switch
	GETSET(int, lineType, LineType);					// SOLID -> USERLINE
	GETSET(int, lineColour, LineColour);				// colour to draw
	GETSET(int, lineWidth, LineWidth);					// width of lines
	GETSET(LineStyle, userLineType, UserLineType);		// User defined line
	GETSET(int, markerType, MarkerType);				// MRKR_{DOT -> DIAMOND}
	GETSET(int, markerColour, MarkerColour);			// colour for markers
	GETSET(int, markerHeight, MarkerHeight);			// pixel size of marker
	GETSET(int, fontId, FontId);						// font id to use
	GETSET(int, textColour, TextColour);				// colour index for text
	GETSET(int, textHAlign, TextHAlign);				// Text horiz alignment
	GETSET(int, textVAlign, TextVAlign);				// Text vert alignment
	GET(int, textHeight);								// Text height
	GETSET(int, textRotation, TextRotation);			// Text rotation angle
	GETSET(int, textEffect, TextEffect);				// Text drawing effect
	GETSET(int, fillType, FillType);					// type of fill to use
	GETSET(int, fillStyle, FillStyle);					// style of fill to use
	GETSET(int, fillColour, FillColour);				// colour index for fill
	GETSET(bool, drawPerimeter, DrawPerimeter);			// Draw perim or not
	GETSET(int, coordType, CoordType);					// NDC or Raster co-ords
	GETSET(int, pageSize, PageSize);					// Page size to use
	GETSET(int, wrMode, WrMode);						// Composition mode
	GETSET(QRect, clip, Clip);							// Clipping rectangle
	GETSET(bool, enableClip, EnableClip);				// Clipping enabled
	GETSET(int, startCap, StartCap);					// Cap-style of start
	GETSET(int, endCap, EndCap);						// Cap-style of end
	GETSET(int, inputModes, InputModes);				// Request (bit=1)
	GETSET(int, activeEvents, ActiveEvents);			// Which events to send
	GETSETP(StringMap*, environment, Environment);		// App environment

	private:
		/*********************************************************************\
		|* Socket to communicate over
		\*********************************************************************/
		QLocalSocket *_client;			// Connection to client
		QColor _palette[256];			// First 256 colours
		QFontMetrics* _fm;				// Font metrics of current font

		/*********************************************************************\
		|* Initialise an instance
		\*********************************************************************/
		void _initialise(void);

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit Workstation(QObject *parent = nullptr);
		explicit Workstation(QLocalSocket *client,
							 QObject *parent = nullptr);
		~Workstation(void);

		/*********************************************************************\
		|* Get/Set colours in the palette
		\*********************************************************************/
		inline void setColour(uint32_t idx,
							  uint8_t r,
							  uint8_t g,
							  uint8_t b,
							  uint8_t a = 255)
			{
			if (idx < 256)
				_palette[idx] = QColor(r,g,b,a);
			}
		inline void setColour(uint32_t idx, QColor c)
			{
			if (idx < 256)
				_palette[idx] = c;
			}

		inline QColor colour(int idx)
			{
			if (idx < 256)
				return _palette[idx];
			return QColor(0,0,0,255);
			}

		/*********************************************************************\
		|* Set the current font and font-metrics
		\*********************************************************************/
		bool setFont(int fontId);
		void setTextHeight(int height);

		/*********************************************************************\
		|* Get the current font-metrics
		\*********************************************************************/
		inline QFontMetrics * fm() { return _fm; }

		/*********************************************************************\
		|* Set up the input mode for various devices
		\*********************************************************************/
		void setInputMode(int device, int mode);
		int16_t inputMode(int device);

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		void setWritingMode(QPainter& painter);

		/*********************************************************************\
		|* Set up the pen for drawing based on the local state
		\*********************************************************************/
		void setupPenForLine(QPen& pen);
		void setupPenForMarker(QPen& pen);
		void setupPenForText(QPen& pen);
		void setupPenForFill(QPen& pen);

		/*********************************************************************\
		|* Return the current 256-entry palette
		\*********************************************************************/
		bool colourPalette(int16_t *rgb);
		bool colourTable(QList<QRgb>& palette);

		/*********************************************************************\
		|* Return a handle id
		\*********************************************************************/
		inline qintptr handle(void)
			{
			if (_client != nullptr)
				return _client->socketDescriptor();
			return 0;
			}

		/*********************************************************************\
		|* Search for an environment variable
		\*********************************************************************/
		bool findEnvironmentVar(const std::string& name, std::string&value);

		/*********************************************************************\
		|* Return the socket
		\*********************************************************************/
		inline QLocalSocket * client(void) { return _client; }

		/*********************************************************************\
		|* Send a message down the socket
		\*********************************************************************/
		void send(ClientMsg* msg, bool log=false);

	signals:


	};

#endif // WORKSTATION_H

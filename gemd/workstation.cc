#include <QColor>

#include "clientmsg.h"
#include "fillfactory.h"
#include "fontmgr.h"
#include "gem.h"
#include "macros.h"
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
			,_textAlign(0)
			,_textHeight(12)
			,_textRotation(0)
			,_textEffect(0)
			,_fillType(FIS_SOLID)
			,_fillStyle(PT_DOTS1)
			,_fillColour(G_BLUE)
			,_drawPerimeter(true)
			,_coordType(COORD_RASTER)
			,_pageSize(PG_DIN_A4)
			,_wrMode(WR_REPLACE)
			,_clip(QRect(0,0,0,0))
			,_enableClip(false)
			,_startCap(CAP_SQUARE)
			,_endCap(CAP_SQUARE)
			,_inputModes(0)
			,_activeEvents(0)
			,_client(client)
			,_fm(nullptr)
	{
	setDefaultColours();
	setFont(-1);
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
	,_textAlign(0)
	,_textHeight(12)
	,_textRotation(0)
	,_textEffect(0)
	,_fillType(FIS_SOLID)
	,_fillStyle(PT_DOTS1)
	,_fillColour(G_BLUE)
	,_drawPerimeter(true)
	,_coordType(COORD_RASTER)
	,_pageSize(PG_DIN_A4)
	,_wrMode(WR_REPLACE)
	,_clip(QRect(0,0,0,0))
	,_enableClip(false)
	,_startCap(CAP_SQUARE)
	,_endCap(CAP_SQUARE)
	,_inputModes(0)
	,_activeEvents(0)
	,_client(nullptr)
	,_fm(nullptr)
	{
	setDefaultColours();
	setFont(-1);
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
|* Set up the font and query the font metrics for it
\*****************************************************************************/
bool Workstation::setFont(int fontId)
	{
	bool ok = false;

	QFont *font = FontMgr::sharedInstance().fetch(fontId);
	if (font != nullptr)
		{
		ok = true;
		_currentFont = *font;
		if (_fm)
			DELETE(_fm);
		_fm = new QFontMetrics(_currentFont);
		_fontId = fontId;
		}

	return ok;
	}

/*****************************************************************************\
|* Set the text height and re-create the metrics
\*****************************************************************************/
void Workstation::setTextHeight(int height)
	{
	_textHeight = height;
	_currentFont.setPixelSize(height);
	if (_fm)
		DELETE(_fm);
	_fm = new QFontMetrics(_currentFont);
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
void Workstation::setupPenForFill(QPen& pen)
	{
	FillFactory& ff = FillFactory::sharedInstance();

	pen.setColor(_palette[_fillColour]);
	switch (_fillType)
		{
		case FIS_HOLLOW:
			break;
		case FIS_SOLID:
			pen.setBrush(_palette[_fillColour]);
			break;
		default:
				{
				QImage& img = ff.patternFor(_fillType, _fillStyle);
				if (img.colorCount() == 2)
					{
					if (_wrMode == WR_TRANSPARENT)
						img.setColor(0, qRgba(0,0,0,0));
					else
						img.setColor(0, _palette[0].rgba());
					img.setColor(1, _palette[_fillColour].rgba());
					}
				pen.setBrush(img);
				}
			break;
		}
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

/*****************************************************************************\
|* Set up the pen for drawing based on the local state
\*****************************************************************************/
void Workstation::setupPenForText(QPen& pen)
	{
	pen.setStyle(_styles[SOLID]);

	QColor c = _palette[_textColour];
	if (_textEffect & TXT_LIGHT)
		c = c.lighter();

	pen.setColor(c);
	pen.setBrush(c);
	pen.setWidth(1);
	}

/*****************************************************************************\
|* Helper routine to figure out the horizontal alignment constant
\*****************************************************************************/
int16_t Workstation::horizontalTextAlignment(void)
	{
	return ((_textAlign & Qt::AlignHCenter) != 0)	? ALGN_CENTER
		   : ((_textAlign & Qt::AlignRight) != 0)	? ALGN_RIGHT
		   : ALGN_LEFT;
	}

/*****************************************************************************\
|* Helper routine to figure out the horizontal alignment constant
\*****************************************************************************/
int16_t Workstation::verticalTextAlignment(void)
	{
	return ((_textAlign & Qt::AlignTop) != 0)		? ALGN_TOP
		   : ((_textAlign & Qt::AlignBottom) != 0)  ? ALGN_BOTTOM
		   : ALGN_BASELINE;
	}

/*****************************************************************************\
|* Set up whether to sample or request information from the various devices
\*****************************************************************************/
void Workstation::setInputMode(int device, int mode)
	{
	if ((device >= INP_LOCATOR) && (device <= INP_STRING))
		{
		if (mode == INPUT_REQUEST)
			_inputModes |= (1<<device);
		else if (mode == INPUT_SAMPLE)
			_inputModes &= ~(1<<device);
		}
	}

/*********************************************************************\
|* Set up the writing mode
\*********************************************************************/
void Workstation::setWritingMode(QPainter& painter)
	{
	switch (_wrMode)
		{
		case WR_REPLACE:
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			break;
		case WR_TRANSPARENT:
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			break;
		case WR_XOR:
			painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
			break;
		case WR_REV_TRANS:	// Life is too short to figure out how this maps...
			painter.setCompositionMode(QPainter::RasterOp_NotSourceAndDestination);
			break;
		}
	}

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QProcessEnvironment>

#include <cstring>

#include "clientmsg.h"
#include "fillfactory.h"
#include "fontmgr.h"
#include "gemTypes.h"
#include "macros.h"
#include "vdi.h"
#include "workstation.h"


#define PALETTE_OFFSET			"/System/Palettes/"
#define SYSTEM_PALETTE_NAME		"system.pal"


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

static const short _devtovdi8[256] =
	{
	0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11,14,12,15,13,255,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,1
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
			,_textHAlign(TA_LEFT)
			,_textVAlign(TA_BASE)
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
	_initialise();
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
	,_textHAlign(TA_LEFT)
	,_textVAlign(TA_BASE)
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
	,_environment(nullptr)
	,_client(nullptr)
	,_fm(nullptr)
	{
	_initialise();
	}


/*****************************************************************************\
|* Destructor
\*****************************************************************************/
Workstation::~Workstation(void)
	{
	DELETE(_environment);
	}


/*****************************************************************************\
|* Initialise the class instance
\*****************************************************************************/
void Workstation::_initialise(void)
	{
	/*************************************************************************\
	|* Read the palette file
	\*************************************************************************/
	std::string pPath = VDI::sharedInstance().rootDir() + PALETTE_OFFSET
					  + SYSTEM_PALETTE_NAME;

	QFile file(QString(pPath.c_str()));
	int size	= (int)file.size();

	if (file.open(QIODeviceBase::ReadOnly))
		{
		uint8_t data[size];
		if (file.read((char *)data, size) != size)
			{
			WARN("Cannot read palette at %s", pPath.c_str());
			return;
			}

		uint16_t * rgb	= (strncmp((char *)data, "PA01", 4) == 0)
						? (uint16_t *)(data + 4)
						: (uint16_t *)data;


		for (int i=0; i<256; i++)
			{
			uint8_t r = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			uint8_t g = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			uint8_t b = ((int)(ntohs(*rgb)) * 255) / 1000; rgb ++;
			setColour(i, r, g, b);
			}
		}
	else
		WARN("Cannot open palette file %s", pPath.c_str());

	/*************************************************************************\
	|* Set the font to be the system font, whatever that may be
	\*************************************************************************/
	setFont(-1);

	/*************************************************************************\
	|* Define a default user line style
	\*************************************************************************/
	_userLineType << 3 << 1;

	/*************************************************************************\
	|* Copy over the current environment
	\*************************************************************************/
	_environment = new StringMap();
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	for (QString& key : env.keys())
		_environment->insert(key, env.value(key));
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
|* Return the palette in VDI form
\*****************************************************************************/
bool Workstation::colourPalette(int16_t *rgb)
	{
	int idx = 0;
	for (int i=0; i<256; i++)
		{
		rgb[idx++] = (_palette[i].red() * 1000) / 256;
		rgb[idx++] = (_palette[i].green() * 1000) / 256;
		rgb[idx++] = (_palette[i].blue() * 1000) / 256;
		}
	return true;
	}

/*****************************************************************************\
|* Return the palette in QT form
\*****************************************************************************/
bool Workstation::colourTable(QList<QRgb>& palette)
	{
	palette.clear();
	palette.reserve(257);
	for (int i=0; i<256; i++)
		palette.append(_palette[_devtovdi8[i]].rgb());
	return true;
	}

///*****************************************************************************\
//|* Set the default colours for the workstation
//\*****************************************************************************/
//void Workstation::setDefaultColours(void)
//	{
//	setColour(0,  255, 255, 255);		// white
//	setColour(1,  0,   0,   0);			// black
//	setColour(2,  255, 0,   0);			// red
//	setColour(3,  0,   255, 0);			// green
//	setColour(4,  0,   0,   255);		// blue
//	setColour(5,  0,   255, 255);		// cyan
//	setColour(6,  255, 255, 0);			// yellow
//	setColour(7,  255, 0,   255);		// magenta
//	setColour(8,  182, 182, 182);		// grey
//	setColour(9,  128, 128, 128);		// dark grey
//	setColour(10, 255, 109, 109);		// light red
//	setColour(11, 109, 109, 255);		// light green
//	setColour(12, 109, 255, 109);		// light blue
//	setColour(13, 109, 255, 255);		// light cyan
//	setColour(14, 255, 255, 109);		// light yellow
//	setColour(15, 255, 109, 255);		// light magenta
//	}

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
		pen.setDashPattern(_userLineType);
	else
		pen.setStyle(_styles[_lineType]);

	pen.setColor(_palette[_lineColour]);
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

	QColor c = _palette[_devtovdi8[_textColour]];
	if (_textEffect & TXT_LIGHT)
		c = c.lighter();

	pen.setColor(c);
	pen.setBrush(c);
	pen.setWidth(1);
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

/*****************************************************************************\
|* Return whether to sample or request information from the various devices
\*****************************************************************************/
int16_t Workstation::inputMode(int device)
	{
	int mode = -1;
	if ((device >= INP_LOCATOR) && (device <= INP_STRING))
		{
		int mask = 1 << device;
		mode = ((_inputModes & mask) != 0) ? INPUT_REQUEST : INPUT_SAMPLE;
		}
	return mode;
	}

/*****************************************************************************\
|* Set up the writing mode
\*****************************************************************************/
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

/*****************************************************************************\
|* Find an environment variable
\*****************************************************************************/
bool Workstation::findEnvironmentVar(const QString& name, QString& value)
	{
	bool found = false;

	if (_environment->contains(name))
		{
		found = true;
		value = _environment->value(name);
		}

	return found;
	}

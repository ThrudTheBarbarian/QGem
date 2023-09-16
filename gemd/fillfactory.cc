#include "fillfactory.h"
#include "gem.h"

#define PATTERN(x,y)   (((x) * 10000) + y)


/*****************************************************************************\
|* Bitmap patterns
\*****************************************************************************/

static unsigned char _errorBits[] = {
	0x00, 0x00, 0x10, 0x08, 0x38, 0x1c, 0x7c, 0x3e, 0xfe, 0x7f, 0x7c, 0x3e,
	0x78, 0x1e, 0x70, 0x0e, 0xf0, 0x0f, 0x78, 0x1e, 0x7c, 0x3e, 0xfe, 0x7f,
	0x7c, 0x3e, 0x38, 0x1c, 0x10, 0x08, 0x00, 0x00};


static unsigned char _pt2_Dots1[] = {
	0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
	0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
	0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00};

static unsigned char _pt2_Dots2[] = {
	0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00,
	0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00,
	0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00};

static unsigned char _pt2_Lines1[] = {
	0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11,
	0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44,
	0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44};

static unsigned char _pt2_Dots_Dark[] = {
	0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
	0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
	0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55};

static unsigned char _pt2_Dots_VDark[] = {
	0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa, 0xdd, 0xdd, 0xaa, 0xaa, 0x77, 0x77,
	0xaa, 0xaa, 0xdd, 0xdd, 0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa, 0xdd, 0xdd,
	0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa, 0xdd, 0xdd};

static unsigned char _pt2_Lines1_Dark[] = {
	0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55,
	0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55,
	0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55};

static unsigned char _pt2_Lines1_VDark[] = {
	0xee, 0xee, 0xff, 0xff, 0xbb, 0xbb, 0xff, 0xff, 0xee, 0xee, 0xff, 0xff,
	0xbb, 0xbb, 0xff, 0xff, 0xee, 0xee, 0xff, 0xff, 0xbb, 0xbb, 0xff, 0xff,
	0xee, 0xee, 0xff, 0xff, 0xbb, 0xbb, 0xff, 0xff};

static unsigned char _pt2_Solid[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static unsigned char _pt2_Bricks[] = {
	0xff, 0xff, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03,
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
	0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xff, 0xff};

static unsigned char _pt2_Bricks_Diag[] = {
	0x05, 0x05, 0x88, 0x88, 0x50, 0x50, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08,
	0x04, 0x04, 0x02, 0x02, 0x05, 0x05, 0x88, 0x88, 0x50, 0x50, 0x20, 0x20,
	0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02};

static unsigned char _pt2_Circumflex[] = {
	0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0xa0, 0xa0, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x04, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40,
	0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04};

static unsigned char _pt2_Shells[] = {
	0x41, 0x41, 0x80, 0x80, 0x80, 0x80, 0xaa, 0xaa, 0x14, 0x14, 0x08, 0x08,
	0x08, 0x08, 0xaa, 0xaa, 0x41, 0x41, 0x80, 0x80, 0x80, 0x80, 0xaa, 0xaa,
	0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0xaa, 0xaa};

static unsigned char _pt2_Diag_Dash[] = {
	0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x00, 0x00, 0x10, 0x10, 0x08, 0x08,
	0x04, 0x04, 0x00, 0x00, 0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x00, 0x00,
	0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x00, 0x00};

static unsigned char _pt2_Ugly[] = {
	0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x00, 0x00, 0x10, 0x10, 0x08, 0x08,
	0x04, 0x04, 0x00, 0x00, 0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x00, 0x00,
	0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x00, 0x00};

static unsigned char _pt2_Dots_Sparse[] = {
	0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char _pt2_Weave[] = {
	0xc7, 0xc7, 0xef, 0x6e, 0x7e, 0x3c, 0x3f, 0x1e, 0x1f, 0x8f, 0xbb, 0xdb,
	0xf1, 0xf1, 0xe3, 0xe3, 0xc7, 0xc7, 0xef, 0x6e, 0x7e, 0x3c, 0x3f, 0x1e,
	0x1f, 0x8f, 0xbb, 0xdb, 0xf1, 0xf1, 0xe3, 0xe3};

static unsigned char _pt2_Line_Dot[] = {
	0x22, 0x22, 0x05, 0x05, 0x88, 0x88, 0x50, 0x50, 0x22, 0x22, 0x00, 0x00,
	0xaa, 0xaa, 0x00, 0x00, 0x22, 0x22, 0x05, 0x05, 0x88, 0x88, 0x50, 0x50,
	0x22, 0x22, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00};

static unsigned char _pt2_Square_Dot[] = {
	0x55, 0x55, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
	0x40, 0x40, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
	0x44, 0x44, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00};

static unsigned char _pt2_Balls[] = {
	0x1f, 0x1f, 0x1f, 0x1f, 0xee, 0xee, 0x91, 0x91, 0xf1, 0xf1, 0xf1, 0xf1,
	0xee, 0xee, 0x19, 0x19, 0x1f, 0x1f, 0x1f, 0x1f, 0xee, 0xee, 0x91, 0x91,
	0xf1, 0xf1, 0xf1, 0xf1, 0xee, 0xee, 0x19, 0x19};

static unsigned char _pt2_Roof1[] = {
	0x02, 0x02, 0x05, 0x05, 0xf8, 0xf8, 0x20, 0x20, 0x20, 0x20, 0x50, 0x50,
	0x8f, 0x8f, 0x02, 0x02, 0x02, 0x02, 0x05, 0x05, 0xf8, 0xf8, 0x20, 0x20,
	0x20, 0x20, 0x50, 0x50, 0x8f, 0x8f, 0x02, 0x02};

static unsigned char _pt2_Roof2[] = {
	0x21, 0x21, 0x12, 0x12, 0x0c, 0x0c, 0x30, 0x30, 0x40, 0x40, 0x80, 0x80,
	0x80, 0x80, 0xc0, 0xc0, 0x21, 0x21, 0x12, 0x12, 0x0c, 0x0c, 0x30, 0x30,
	0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0};

static unsigned char _pt2_Checker[] = {
	0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0x1e, 0x1e, 0x1e, 0x1e,
	0x1e, 0x1e, 0x1e, 0x1e, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1,
	0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e};

static unsigned char _pt2_Diamond[] = {
	0xc1, 0xc1, 0xe3, 0xe3, 0xf7, 0xf7, 0xff, 0xff, 0xf7, 0xf7, 0xe3, 0xe3,
	0xc1, 0xc1, 0x80, 0x80, 0xc1, 0xc1, 0xe3, 0xe3, 0xf7, 0xf7, 0xff, 0xff,
	0xf7, 0xf7, 0xe3, 0xe3, 0xc1, 0xc1, 0x80, 0x80};

static unsigned char _pt2_Chevron[] = {
	0x88, 0x88, 0x44, 0x44, 0xff, 0xff, 0x22, 0x22, 0x44, 0x44, 0x88, 0x88,
	0xff, 0xff, 0x11, 0x11, 0x88, 0x88, 0x44, 0x44, 0xff, 0xff, 0x22, 0x22,
	0x44, 0x44, 0x88, 0x88, 0xff, 0xff, 0x44, 0x44};

static unsigned char _pt3_Diag1[] = {
	0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08,
	0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20,
	0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02};

static unsigned char _pt3_Diag2[] = {
	0x03, 0x03, 0x81, 0x81, 0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18,
	0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x81, 0x81, 0xc0, 0xc0, 0x60, 0x60,
	0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06};

static unsigned char _pt3_Cross_Hatch[] = {
	0x81, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24,
	0x42, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18,
	0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x81, 0x81};

static unsigned char _pt3_Vertical[] = {
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

static unsigned char _pt3_Horizontal[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00};

static unsigned char _pt3_Square[] = {
	0x02, 0x02, 0xff, 0xff, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xff, 0xff, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

static unsigned char _pt3_Diag3[] = {
	0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08,
	0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20,
	0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02};

static unsigned char _pt3_Diag3_Bold[] = {
	0x03, 0x03, 0x81, 0x81, 0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18,
	0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x81, 0x81, 0xc0, 0xc0, 0x60, 0x60,
	0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06};

static unsigned char _pt3_Cross_Hatch2[] = {
	0x81, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24,
	0x42, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18,
	0x18, 0x18, 0x24, 0x24, 0x42, 0x42, 0x81, 0x81};

static unsigned char _pt3_Vertical2[] = {
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};

static unsigned char _pt3_Horizontal2[] = {
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static unsigned char _pt3_Square2[] = {
	0xff, 0xff, 0x11, 0x11, 0x55, 0x55, 0x11, 0x11, 0xff, 0xff, 0x11, 0x11,
	0x55, 0x55, 0x11, 0x11, 0xff, 0xff, 0x11, 0x11, 0x55, 0x55, 0x11, 0x11,
	0xff, 0xff, 0x11, 0x11, 0x55, 0x55, 0x11, 0x11};

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
FillFactory::FillFactory(QObject *parent)
	: QObject{parent}
	{
	// Create an error pixmap and store it as {6,0}
	_create(6, 0, _errorBits);
	}


/*****************************************************************************\
|* Return a pixmap for a given pattern and type
\*****************************************************************************/
QImage& FillFactory::patternFor(int type, int style)
	{
	/*************************************************************************\
	|* Check to see if we've already done it
	\*************************************************************************/
	int patternId = PATTERN(type, style);
	if (_patterns.contains(patternId))
		return _patterns[patternId];

	/*************************************************************************\
	|* See if it's a recognised variant
	\*************************************************************************/
	uint8_t *pattern = nullptr;

	switch (type)
		{
		case FIS_PATTERN:
			switch (style)
				{
				case PT_DOTS1:
					pattern = _pt2_Dots1;
					break;
				case PT_DOTS2:
					pattern = _pt2_Dots2;
					break;
				case PT_LINES1:
					pattern = _pt2_Lines1;
					break;
				case PT_DOTS_DARK:
					pattern = _pt2_Dots_Dark;
					break;
				case PT_DOTS_VDARK:
					pattern = _pt2_Dots_VDark;
					break;
				case PT_LINES1_DARK:
					pattern = _pt2_Lines1_Dark;
					break;
				case PT_LINES1_VDARK:
					pattern = _pt2_Lines1_VDark;
					break;
				case PT_SOLID:
					pattern = _pt2_Solid;
					break;
				case PT_BRICKS:
					pattern = _pt2_Bricks;
					break;
				case PT_BRICKS_DIAG:
					pattern = _pt2_Bricks_Diag;
					break;
				case PT_CIRCUMFLEX:
					pattern = _pt2_Circumflex;
					break;
				case PT_SHELLS:
					pattern = _pt2_Shells;
					break;
				case PT_DIAG_DASH:
					pattern = _pt2_Diag_Dash;
					break;
				case PT_UGLY:
					pattern = _pt2_Ugly;
					break;
				case PT_DOTS_SPARSE:
					pattern = _pt2_Dots_Sparse;
					break;
				case PT_WEAVE:
					pattern = _pt2_Weave;
					break;
				case PT_LINE_DOT:
					pattern = _pt2_Line_Dot;
					break;
				case PT_SQUARE_DOT:
					pattern = _pt2_Square_Dot;
					break;
				case PT_BALLS:
					pattern = _pt2_Balls;
					break;
				case PT_ROOF1:
					pattern = _pt2_Roof1;
					break;
				case PT_ROOF2:
					pattern = _pt2_Roof2;
					break;
				case PT_CHECKER:
					pattern = _pt2_Checker;
					break;
				case PT_DIAMONDS:
					pattern = _pt2_Diamond;
					break;
				case PT_CHEVRON:
					pattern = _pt2_Chevron;
					break;
				default:
					return _patterns[PATTERN(6,0)];
				}
			break;

		case FIS_HATCH:
			switch (style)
				{
				case PT_DIAG1:
					pattern = _pt3_Diag1;
					break;
				case PT_DIAG2:
					pattern = _pt3_Diag2;
					break;
				case PT_CROSS_HATCH:
					pattern = _pt3_Cross_Hatch;
					break;
				case PT_VERTICAL:
					pattern = _pt3_Vertical;
					break;
				case PT_HORIZONTAL:
					pattern = _pt3_Horizontal;
					break;
				case PT_SQUARE:
					pattern = _pt3_Square;
					break;
				case PT_DIAG3:
					pattern = _pt3_Diag3;
					break;
				case PT_DIAG3_BOLD:
					pattern = _pt3_Diag3_Bold;
					break;
				case PT_CROSS_HATCH2:
					pattern = _pt3_Cross_Hatch2;
					break;
				case PT_VERTICAL2:
					pattern = _pt3_Vertical2;
					break;
				case PT_HORIZONTAL2:
					pattern = _pt3_Horizontal2;
					break;
				case PT_SQUARE2:
					pattern = _pt3_Square2;
					break;
				default:
					return _patterns[PATTERN(6,0)];
				}
			break;

		default:
			return _patterns[PATTERN(6,0)];
		}

	return _create(type, style, pattern);
	}


#pragma mark - Private Methods

/*****************************************************************************\
|* Return an image for a given pattern and type
\*****************************************************************************/
QImage& FillFactory::_create(int type, int style, uint8_t *src, int planes)
	{
	int patternId = PATTERN(type, style);

	if (planes == 1)
		{
		QImage img(src, 16, 16, 2, QImage::Format_Mono);
		img.setColorCount(2);
		_patterns[patternId] = img;
		}
	return _patterns[patternId];
	}

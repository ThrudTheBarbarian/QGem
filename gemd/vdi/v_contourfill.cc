#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "fillfactory.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static int * _allocateStack(void);
static uint8_t ** _allocateVisits(int w, int h);
static void _tile(QImage &src, QImage *dst);
static void _fill(int x, int y, int w, int h,
				  uint32_t ** src, uint32_t ** dst);
void _filluntil(int x, int y, int w, int h,
				uint32_t fillColour, uint32_t ** src, uint32_t ** dst);

/*****************************************************************************\
|* Defines and constants
\*****************************************************************************/
#define STACK_SIZE 16777216


#define PUSH(a,b,c,d)						\
	do {									\
		stack[SP++] = a;					\
		stack[SP++]	= b;					\
		stack[SP++] = c;					\
		stack[SP++] = d;					\
	} while (0)

#define POP(a,b,c,d)						\
		do {								\
		a = stack[j++];						\
		b = stack[j++];						\
		c = stack[j++];						\
		d = stack[j++];						\
	} while (0)

#define COPY_RGB(x, y)						\
	do	{									\
		dst[y][x] = src[y][x];				\
		visited[y][x] = 1;					\
		} while (0)

#define DST(x,y)	dst[y][x]
#define SEEN(x,y)	visited[y][x]


/*****************************************************************************\
|* Opcode 103: Flood fill an area
|*
|* Original signature is: v_contourfill(int16_t handle,
|*										int16_t x,
|*										int16_t y,
|*										int16_t colour);
|*
\*****************************************************************************/
void VDI::v_contourfill(qintptr handle, int16_t x, int16_t y, int16_t colour)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QPainter painter(_img);

	if (ws != nullptr)
		{
		/*********************************************************************\
		|* Get the dimensions of the destination image (the screen image)
		\*********************************************************************/
		int W = _img->width();
		int H = _img->height();
		int h;

		/*********************************************************************\
		|* Create an RGB32 image that is the same width as the destination
		|* image, but has the height of the source image (or solid colour)
		|* and tile it with the source image (or solid colour)
		\*********************************************************************/
		QImage *pattern = nullptr;

		int type = ws->fillType();
		switch (type)
			{
			case FIS_SOLID:
				{
				h = 2;
				pattern = new QImage(_screen->width(), h, QImage::Format_RGB32);
				QPainter subPainter(pattern);
				QColor bg = ws->colour(colour);
				subPainter.fillRect(QRect(0,0,_screen->width(), h), bg);
				break;
				}

			case FIS_USER:
				{
				QImage& from = FillFactory::sharedInstance().patternFor(type, 0);
				h = from.height();
				pattern = new QImage(_screen->width(), h, QImage::Format_RGB32);
				_tile(from, pattern);
				break;
				}
			case FIS_HATCH:
			case FIS_PATTERN:
				{
				int style = ws->fillStyle();
				QImage& from = FillFactory::sharedInstance().patternFor(type, style);
				h = from.height();
				pattern = new QImage(_screen->width(), h, QImage::Format_RGB32);
				_tile(from, pattern);
				break;
				}
			default:
				return;
			}

		/*********************************************************************\
		|* Create pointers to the scanline data that map 1:1 to the y co-ords
		|* of the image to fill (the screen), even if the destination is not
		|* of the same size
		\*********************************************************************/
		uint32_t ** src = new uint32_t * [H];
		uint32_t ** dst = new uint32_t * [H];

		for (int i=0; i<H; i++)
			{
			src[i] = (uint32_t *) pattern->scanLine(i%h);
			dst[i] = (uint32_t *) _img->scanLine(i);
			}

		/*********************************************************************\
		|* If the fill-index is negative, flood-fill from the point (x,y) over
		|* any connected pixels which are the same colour
		\*********************************************************************/
		if (colour < 0)
			{
			_fill(x, y, W, H, src, dst);
			}

		/*********************************************************************\
		|* Otherwise get the colour from the index into the palette, and flood
		|* fill everything until we reach that colour
		\*********************************************************************/
		else
			{
			QColor toFill = ws->colour(colour);
			uint32_t c = toFill.rgb();
			_filluntil(x, y, W, H, c, src, dst);
			}

		/*********************************************************************\
		|* Tidy up
		\*********************************************************************/
		DELETE_ARRAY(src);
		DELETE_ARRAY(dst);
		DELETE(pattern);
		}
	}

/*****************************************************************************\
|* And from the socket interface
|*
|* Opcode 9:	Fill a polygon.			[type=0] [pxy=...]
\*****************************************************************************/
void VDI::v_contourfill(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);
	int16_t colour		= ntohs(p[2]);

	v_contourfill(ws->handle(), x, y, colour);
	}

#pragma mark - Helper functions

/*****************************************************************************\
|* Flood-fill the image
\*****************************************************************************/
void _filluntil(int x, int y, int w, int h,
				uint32_t fillColour, uint32_t ** src, uint32_t ** dst)
	{
	/*********************************************************************\
	|* Allocate a working stack
	\*********************************************************************/
	int SP = 0;							// Initialise the stack pointer
	int *stack = _allocateStack();
	if (stack == nullptr)
		return;

	/*********************************************************************\
	|* Allocate a visited array
	\*********************************************************************/
	uint8_t **visited = _allocateVisits(w, h);
	if (visited == nullptr)
		{
		DELETE_ARRAY(stack);
		return;
		}

	/*********************************************************************\
	|* Scan left and right to get seeds
	\*********************************************************************/
	int x1 = x;
	while ((x1 > -1) && (DST(x1,y) != fillColour))
		x1--;
	x1++;

	int x2 = x;
	while ((x2 < h) && (DST(x2,y) != fillColour))
		x2++;
	x2--;

	PUSH(x1, x2, y, 1);
	PUSH(x1, x2, ++y, -1);

	/*********************************************************************\
	|* Operate until the stack is empty
	\*********************************************************************/
	for (int j = 0; j < SP;)
		{
		int x3, x4, y1, y2;
		POP(x3, x4, y1, y2);

		y1 += y2;
		if ((y1 >= 0) && (y1 < h))
			{
			int x5 = x3;

			while ((x5 > -1) && (DST(x5,y1) != fillColour) && !SEEN(x5, y1))
				x5--;
			x5++;

			int x6;
			if (x5 <= x3)
				{
				x6 = x3 + 1;

				while ((x6 < w) && (DST(x6,y1) != fillColour) && !SEEN(x6,y1))
				x6++;
				x6--;

				for (int i = x5; i <= x6; i++)
				COPY_RGB(i,y1);

				if (x3 - 1 > x5)
				PUSH(x5, x3-1, y1, -y2);

				if (x6 > x4 + 1)
				PUSH(x4+1,x6,y1, -y2);

				PUSH(x5, x6, y1, y2);
				}
			else
				x6 = x3;

			while (x6 < x4)
				{
				x6++;
				x5 = x6;
				while ((x6 < w) && (DST(x6,y1) != fillColour) && !SEEN(x6,y1))
				x6++;

				if (x6 > x5)
					{
					x6--;

					for (int i = x5; i <= x6; i++)
						COPY_RGB(i, y1);

					if (x6 > x4 + 1)
						PUSH(x4+1, x6, y1, -y2);

					PUSH(x5, x6, y1, y2);
					}
				}
			}
		}

	/*********************************************************************\
	|* Clean up the memory
	\*********************************************************************/
	DELETE_ARRAY(stack);
	for (int i=0; i<h; i++)
		DELETE_ARRAY(visited[i]);
	DELETE_ARRAY(visited);
	}

/*****************************************************************************\
|* Flood-fill the image
\*****************************************************************************/
void _fill(int x, int y, int w, int h, uint32_t ** src, uint32_t ** dst)
	{
	uint32_t oldColour	= dst[y][x];		// Colour to look for

	/*********************************************************************\
	|* Allocate a working stack
	\*********************************************************************/
	int SP = 0;							// Initialise the stack pointer
	int *stack = _allocateStack();
	if (stack == nullptr)
		return;

	/*********************************************************************\
	|* Allocate a visited array
	\*********************************************************************/
	uint8_t **visited = _allocateVisits(w, h);
	if (visited == nullptr)
		{
		DELETE_ARRAY(stack);
		return;
		}

	/*********************************************************************\
	|* Scan left and right to get seeds
	\*********************************************************************/
	int x1 = x;
	while ((x1 > -1) && (DST(x1,y) == oldColour))
		x1--;
	x1++;

	int x2 = x;
	while ((x2 < h) && (DST(x2,y) == oldColour))
		x2++;
	x2--;

	PUSH(x1, x2, y, 1);
	PUSH(x1, x2, ++y, -1);

	/*********************************************************************\
	|* Operate until the stack is empty
	\*********************************************************************/
	for (int j = 0; j < SP;)
		{
		int x3, x4, y1, y2;
		POP(x3, x4, y1, y2);

		y1 += y2;
		if ((y1 >= 0) && (y1 < h))
			{
			int x5 = x3;

			while ((x5 > -1) && (DST(x5,y1) == oldColour) && !SEEN(x5, y1))
				x5--;
			x5++;

			int x6;
			if (x5 <= x3)
				{
				x6 = x3 + 1;

				while ((x6 < w) && (DST(x6,y1) == oldColour) && !SEEN(x6,y1))
					x6++;
				x6--;

				for (int i = x5; i <= x6; i++)
					COPY_RGB(i,y1);

				if (x3 - 1 > x5)
					PUSH(x5, x3-1, y1, -y2);

				if (x6 > x4 + 1)
					PUSH(x4+1,x6,y1, -y2);

				PUSH(x5, x6, y1, y2);
				}
			else
				x6 = x3;

			while (x6 < x4)
				{
				x6++;
				x5 = x6;
				while ((x6 < w) && (DST(x6,y1) == oldColour) && !SEEN(x6,y1))
					x6++;

				if (x6 > x5)
					{
					x6--;

					for (int i = x5; i <= x6; i++)
					COPY_RGB(i, y1);

					if (x6 > x4 + 1)
						PUSH(x4+1, x6, y1, -y2);

					PUSH(x5, x6, y1, y2);
					}
				}
			}
		}

	/*********************************************************************\
	|* Clean up the memory
	\*********************************************************************/
	DELETE_ARRAY(stack);
	for (int i=0; i<h; i++)
		DELETE_ARRAY(visited[i]);
	DELETE_ARRAY(visited);
	}

/*****************************************************************************\
|* Tile a source image into a destination image
\*****************************************************************************/
static void _tile(QImage& src, QImage *dst)
	{
	int dstW = dst->width();
	int dstH = dst->height();
	int srcW = src.width();
	int srcH = src.height();

	int rows = dstH / srcH;
	if (dstH != srcH)
		rows ++;

	int cols = dstW / srcW;
	if (dstW != srcW)
		cols ++;

	int yc = 0;
	QPainter p(dst);
	for (int y=0; y<rows; y++)
		{
		int xc = 0;
		for (int x=0; x<cols; x++)
			{
			p.drawImage(xc, yc, src);
			xc += srcW;
			}
		yc += srcH;
		}
	}

/*****************************************************************************\
|* Allocate a stack to operate the flood-fill inside
\*****************************************************************************/
static int * _allocateStack(void)
	{
	int *stack = new int[STACK_SIZE];
	if (stack == nullptr)
		WARN("Could not allocate floodfill stack!");
	return stack;
	}


/*****************************************************************************\
|* Allocate a stack to operate the flood-fill inside
\*****************************************************************************/
static uint8_t ** _allocateVisits(int w, int h)
	{
	uint8_t **visited = new uint8_t*[h];
	if (visited == nullptr)
		{
		WARN("Could not allocate floodfill visited top array!");
		}
	else
		{
		for (int i=0; i<h; i++)
			{
			visited[i] = new uint8_t[w];
			if (visited[i] == nullptr)
				{
				WARN("Could not allocated floodfill index array!");
				for (int j=0; j<i-1; j++)
					DELETE_ARRAY(visited[j]);
				DELETE_ARRAY(visited);
				break;
				}
			memset(visited[i], 0, w);
			}
		}
	return visited;
	}


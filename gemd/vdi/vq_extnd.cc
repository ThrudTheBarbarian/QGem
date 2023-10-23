#include "clientmsg.h"
#include "connectionmgr.h"
#include "gemTypes.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

#define CHECK_RANGE(x,a,b)		(((x) < (a)) || ((x) > (b))) ? (a) : (x)
#define CHECK_RANGE2(x,a,b,c)	(((x) < (a)) || ((x) > (b))) ? (c) : (x)
#define CHECK_RANGE3(x,a,b,c,d)	(((x) < (a)) || ((x) > (b))) ? (c) : (d)

/*****************************************************************************\
|* Query for workstation information.
|*
|* Original signature is: vq_extnd(int16_t handle,
|*								   int16_t extend,
|*								   int16_t *workOut		[n ints returned]
|*
\*****************************************************************************/
void VDI::vq_extnd(qintptr handle, int16_t  extend, int16_t* data)
	{
	ConnectionMgr *cm	= _screen->connectionManager();
	Workstation *ws		= cm->findWorkstationForHandle(handle);

	if (ws != nullptr)
		{
		int idx = 0;
		if (extend == 0)
			{
			data[idx++]	= _screen->width()-1;		// xMax
			data[idx++]	= _screen->height()-1;		// yMax
			data[idx++]	= 0;						// Can scale
			data[idx++]	= 18;						// Pixel width in microns
			data[idx++]	= 18;						// Pixel height in microns
			data[idx++]	= 0;						// smooth scaling
			data[idx++]	= USERLINE;					// # line types
			data[idx++]	= 0;						// smooth line widths
			data[idx++]	= MRKR_DIAMOND;				// # marker types
			data[idx++]	= 0;						// smooth marker scaling
			data[idx++]	= 1;						// number of fonts
			data[idx++]	= PT_CHEVRON;				// # pattern types
			data[idx++]	= PT_SQUARE2;				// # hatching styles
			data[idx++]	= 32767;					// # colours
			data[idx++]	= GDP_NONE;					// # GDP types
			data[idx++]	= GDP_BAR;					// rect
			data[idx++]	= GDP_ARC;					// arc
			data[idx++]	= GDP_PIESLICE;				// filled arc
			data[idx++]	= GDP_FILLED_CIRCLE;		// filled circle
			data[idx++]	= GDP_FILLED_ELLIPSE;		// filled ellipse
			data[idx++]	= GDP_ELLIPTICAL_ARC;		// elliptical arc
			data[idx++]	= GDP_ELLIPTICAL_PIE;		// filled ell. arc
			data[idx++]	= GDP_ROUNDED_RECT;			// rounded rect
			data[idx++]	= GDP_FILLED_RRECT;			// filled r.rect
			data[idx++]	= GDP_JUSTIFIED_TEXT;		// justified text
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_LINE;					// attrib
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_LINE;					// attrib
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_LINE;					// attrib
			data[idx++]	= GDP_FILL;					// attrib
			data[idx++]	= GDP_TEXT;					// attrib
			data[idx++]	= 1;						// colour-capable
			data[idx++]	= 1;						// can rotate text
			data[idx++]	= 1;						// can perform fills
			data[idx++]	= 0;						// no cell-array
			data[idx++]	= 0;						// > 32767 colours
			data[idx++]	= 2;						// # locators (keyb,mse)
			data[idx++]	= 1;						// #valuators (arrows)
			data[idx++]	= 1;						// # choice (fns)
			data[idx++]	= 1;						// keyboard present
			data[idx++]	= 2;						// both input & output
			data[idx++]	= 5;						// min char width (pix)
			data[idx++]	= 4;						// min char height
			data[idx++]	= 7;						// max char width
			data[idx++]	= 13;						// max char ht
			data[idx++]	= 1;						// min line width
			data[idx++]	= 0;						// unused
			data[idx++]	= 128;						// max line width
			data[idx++]	= 0;						// unused
			data[idx++]	= 15;						// min marker width
			data[idx++]	= 1;						// min marker ht
			data[idx++]	= 120;						// max marker wd
			data[idx++]	= 88;						// max marker ht
			}
		else
			{
			data[idx++]	= 4;						// combined alpha/gfx
			data[idx++]	= 32767;					// # colours
			data[idx++]	= TXT_MASK;					// # text effects
			data[idx++]	= 1;						// can scale rasters
			data[idx++]	= 24;						// # bitplanes
			data[idx++]	= 1;						// pseudo CLUT
			data[idx++]	= 32767;					// # ops/sec
			data[idx++]	= 1;						// can contour fill
			data[idx++]	= 2;						// smooth text rotate
			data[idx++]	= 4;						// # writing modes
			data[idx++]	= 2;						// can req and sample
			data[idx++]	= 1;						// can text align
			data[idx++]	= 0;						// inking ? Nope
			data[idx++]	= 2;						// rubberband lns/rcts
			data[idx++]	= -1;						// unlimited vertices
			data[idx++]	= -1;						// unlimited intin
			data[idx++]	= 2;						// # mouse buttons
			data[idx++]	= 1;						// line-types for >1
			data[idx++]	= 4;						// wrmodes for >1 lines
			data[idx++]	= 1;						// clipping flag
			data[idx++]	= 0;						// No extended precision
			data[28]    = 1;						// has bezier
			data[30]	= 1;						// vrt/vro_cpyfm can scale
			}
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vq_extnd(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t mode		= ntohs(p[0]);
	int16_t data[57];

	memset(data, 0, sizeof(int16_t) * 57);
	vq_extnd(ws->handle(), mode, data);

	/*************************************************************************\
	|* Create a message to send back
	\*************************************************************************/
	cm->clear();
	cm->setType(MSG_REPLY(ClientMsg::VQ_EXTND));
	cm->append(data, 57);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

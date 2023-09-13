#include "clientmsg.h"
#include "gem.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

#define CHECK_RANGE(x,a,b)		(((x) < (a)) || ((x) > (b))) ? (a) : (x)
#define CHECK_RANGE2(x,a,b,c)	(((x) < (a)) || ((x) > (b))) ? (c) : (x)
#define CHECK_RANGE3(x,a,b,c,d)	(((x) < (a)) || ((x) > (b))) ? (c) : (d)

/*****************************************************************************\
|* Open a virtual workstation.
|*
|* Original signature is: v_opnvwk(int16_t *workIn,		[16 ints input]
|*								   int16_t *handle,		[1 int returned]
|*								   int16_t *workOut		[57 ints returned]
|*
|* To facilitate decode, we place the handle at the end of the workOut array
|* when returning the data
\*****************************************************************************/
void VDI::v_opnvwk(Workstation *ws, ClientMsg *cm)
	{
	/*************************************************************************\
	|* Set any defaults that were sent through (<0 == ignore)
	\*************************************************************************/
	const Payload &p = cm->payload();

	// p[0]:  1 = same resolution, <0 = unset, >1 = change resolution
	ws->setDeviceDriver(CHECK_RANGE(p[0], 1, 6));

	// p[1]:  Default line type (vsl_type)
	ws->setLineType(CHECK_RANGE(p[1], SOLID, USERLINE));

	// p[2]:  Default line colour index
	ws->setLineColour(CHECK_RANGE2(p[2], G_WHITE, G_LMAGENTA, G_BLACK));

	// p[3]:  Default marker type (vsm_type)
	ws->setMarkerType(CHECK_RANGE(p[3], MRKR_DOT, MRKR_DIAMOND));

	// p[4]:  Default marker colour
	ws->setMarkerColour(CHECK_RANGE2(p[4], G_WHITE, G_LMAGENTA, G_BLACK));

	// p[5]:  Font id
	ws->setMarkerColour(CHECK_RANGE(p[5], 0, 1024));

	// p[6]:  Default marker colour
	ws->setTextColour(CHECK_RANGE2(p[6], G_WHITE, G_LMAGENTA, G_BLACK));

	// p[7]:  Type of fill (vsf_interior)
	ws->setFillType(CHECK_RANGE2(p[7], FIS_HOLLOW, FIS_USER, FIS_SOLID));

	// p[8]:  Style to use for filling (vsf_style)
	ws->setFillStyle(CHECK_RANGE(p[8], PT_DOTS1, PT_CHEVRON));

	// p[9]:  Default fill colour
	ws->setFillColour(CHECK_RANGE2(p[9], G_WHITE, G_LMAGENTA, G_BLACK));

	// p[10]:  co-ordinate type
	if ((p[10] == COORD_NDC) || (p[10] == COORD_RASTER))
		ws->setCoordType(p[10]);
	else
		ws->setCoordType(COORD_RASTER);

	// p[11]:  page size
	int pg = p[11] & 0xF;
	ws->setPageSize(CHECK_RANGE3(pg, PG_DEFAULT, PG_DIN_B5, PG_DEFAULT, p[11]));

	// p[12]:  not used
	// p[13]:  pointer to filename or 0L. Ignore for now
	// p[14]:  not used
	// p[15]:  not used

	/*************************************************************************\
	|* Create a message to send back
	\*************************************************************************/
	VDI& vdi = VDI::sharedInstance();
	Screen *s = vdi.screen();

	cm->clear();
	cm->append(s->width()-1);		// 0 : Xmax
	cm->append(s->height()-1);		// 1 : Ymax
	cm->append(0);					// 2 : device can scale
	cm->append(18);					// 3 : pixel width in microns
	cm->append(18);					// 4 : pixel height in microns
	cm->append(0);					// 5 : continuous character height scaling
	cm->append(USERLINE);			// 6 : # line types (1->n)
	cm->append(0);					// 7 : continuous line width scaling
	cm->append(MRKR_DIAMOND);		// 8 : # marker types (1->n)
	cm->append(0);					// 9 : continuous marker height scaling
	cm->append(1);					// 10: number of accessible fonts
	cm->append(PT_CHEVRON);			// 11: # pattern types (1->n)
	cm->append(PT_SQUARE2);			// 12: # hatch-styles (1->n)
	cm->append(32767);				// 13: # of simultaneous colours
	cm->append(GDP_NONE);			// 14: # of generalised drawing types
	cm->append(GDP_BAR);			// 15: Rectangle
	cm->append(GDP_ARC);			// 16: Arc
	cm->append(GDP_PIESLICE);		// 17: Arc + filled to center
	cm->append(GDP_FILLED_CIRCLE);	// 18: Filled circle
	cm->append(GDP_FILLED_ELLIPSE);	// 19: Filled ellipse
	cm->append(GDP_ELLIPTICAL_ARC);	// 20: Elliptical arc
	cm->append(GDP_ELLIPTICAL_PIE);	// 21: Elliptical arc + filled to center
	cm->append(GDP_ROUNDED_RECT);	// 22: Rounded rect
	cm->append(GDP_FILLED_RRECT);	// 23: Filled rounded rect
	cm->append(GDP_JUSTIFIED_TEXT);	// 24: Justified text
	cm->append(GDP_FILL);			// 25: Attribute for rectangle
	cm->append(GDP_LINE);			// 26: Attribute for arc
	cm->append(GDP_FILL);			// 27: Attribute for pieslice
	cm->append(GDP_FILL);			// 28: Attribute for filled circle
	cm->append(GDP_FILL);			// 29: Attribute for filled ellipse
	cm->append(GDP_LINE);			// 30: Attribute for elliptical arc
	cm->append(GDP_FILL);			// 31: Attribute for elliptical pie
	cm->append(GDP_LINE);			// 32: Attribute for rounded rect
	cm->append(GDP_FILL);			// 33: Attribute for filled rounded rect
	cm->append(GDP_TEXT);			// 34: Attribute for text
	cm->append(1);					// 35: we are colour-capable
	cm->append(1);					// 36: we can rotate text
	cm->append(1);					// 37: we can perform fills
	cm->append(0);					// 38: we do not perform cell-array drawing
	cm->append(0);					// 39: we have > 32767 colours
	cm->append(2);					// 40: # locators (keyboard & mouse)
	cm->append(1);					// 41: # valuators (arrow keys on keyboard)
	cm->append(1);					// 42: # choice devices (function keys)
	cm->append(1);					// 43: we have a keyboard
	cm->append(2);					// 44: device is input & output
	cm->append(5);					// 45: minimum character width in pixels
	cm->append(4);					// 46: minimum character height in pixels
	cm->append(7);					// 47: maximum character width in pixels
	cm->append(13);					// 48: maximum character height in pixels
	cm->append(1);					// 49: minimum line width in pixels
	cm->append(0);					// 50: unused
	cm->append(128);				// 51: maximum line width in pixels
	cm->append(0);					// 52: unused
	cm->append(15);					// 53: minimum marker width in pixels
	cm->append(1);					// 54: minimum marker height in pixels
	cm->append(120);				// 55: maximum marker width in pixels
	cm->append(88);					// 56: maximum marker height in pixels
	cm->append(ws->client()->socketDescriptor());
	cm->setType(ClientMsg::MSG_REPLY_OFFSET + ClientMsg::V_OPNVWK);


	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

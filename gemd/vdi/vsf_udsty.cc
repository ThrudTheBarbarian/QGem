#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 113: Set the user-defined line-style
|*
|* Original signature is:
|*
|* void vsf_udsty(int16_t handle, int16_t pat)
|*
\*****************************************************************************/
void VDI::vsf_udsty(qintptr handle, int16_t pat)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	/*********************************************************************\
	|* Convert the 16 bits into a qlist
	\*********************************************************************/
	int last		= -1;
	int increment	= 0;
	int mask		= 0x8000;

	QList<qreal> pattern;
	for (int i=0; i<16; i++)
		{
		int bit = (pat & mask) ? 1 : 0;
		if (bit == last)
			increment ++;
		else
			{
			if (i != 0)
				pattern << increment;
			increment	= 1;
			last		= bit;
			}
		mask >>= 1;
		}
	pattern << increment;
	if (pattern.size() & 1)
		pattern << 1;

	/*********************************************************************\
	|* Check to see if either MFDB points to the screen
	\*********************************************************************/
	ws->setUserLineType(pattern);

	}



/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vsf_udsty(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t style		= ntohs(p[0]);

	vsf_udsty(ws->handle(), style);
	}

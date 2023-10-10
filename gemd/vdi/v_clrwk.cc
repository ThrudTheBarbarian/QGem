#include <QPainter>

#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 3: Clear a physical workstation.
|*
|* Original signature is: v_clrwk(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_clrwk(Workstation *ws)
	{
	QPainter p(_img);
	p.fillRect(0, 0, _screen->width(), _screen->height(), ws->colour(0));
	}

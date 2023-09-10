#include <QPainter>

#include "debug.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 3: Clear a physical workstation.
|*
|* Original signature is: v_clrwk(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_clrwk(int16_t handle)
	{
	if (handle == 0)
		{
		QPainter p(_img);
		p.fillRect(0,0,_screen->width(),_screen->height(),
				   QBrush(_palette[0], Qt::SolidPattern));
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

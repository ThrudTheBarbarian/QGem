#include <QFontMetrics>

#include "debug.h"
#include "screen.h"
#include "vdi.h"


/*****************************************************************************\
|* Opcode 5.1: Query the number of character cells on the alpha display.
|*
|* Original signature is:
|*
|*  vq_chcells(int16_t handle, int16_t& rows, int16_t& columns);
|*
\*****************************************************************************/
void VDI::vq_chcells(int16_t handle, int16_t& rows, int16_t& columns)
	{
	if (handle != 0)
		{
		WARN("Non-screen devices currently unsupported");
		}
	else
		{
		QFontMetrics metrics(_systemFont);
		QRect bounds	= metrics.boundingRect("W");
		_charHeight		= bounds.height();
		_charWidth		= bounds.width();
		rows			= _screen->height() / _charHeight;
		columns			= _screen->width() / _charWidth;
		}
	}

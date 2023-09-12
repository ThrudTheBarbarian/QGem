
#include "debug.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.5: Move the cursor down if possible.
|*
|* Original signature is: v_curdown(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curdown(qintptr handle)
	{
	if (handle == 0)
		{
		int lastRow = _screen->height() / _charHeight;
		if (_cursorY < lastRow - 1)
			{
			bool erased = _eraseCursor();
			_cursorY ++;
			if (erased)
				_drawCursor();
			}
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curdown(Workstation *)
	{
	v_curdown((qintptr)0);
	}


#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.7: Move the cursor left if possible.
|*
|* Original signature is: v_curleft(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curleft(int16_t handle)
	{
	if (handle == 0)
		{
		if (_cursorX > 0)
			{
			bool erased = _eraseCursor();
			_cursorX --;
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
void VDI::v_curleft(Workstation *)
	{
	v_curleft((int16_t)0);
	}

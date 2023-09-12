
#include "debug.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 5.4: Move the cursor up if possible.
|*
|* Original signature is: v_curup(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curup(qintptr handle)
	{
	if (handle == 0)
		{
		if (_cursorY > 0)
			{
			bool erased = _eraseCursor();
			_cursorY --;
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
void VDI::v_curup(Workstation *)
	{
	v_curup((qintptr)0);
	}

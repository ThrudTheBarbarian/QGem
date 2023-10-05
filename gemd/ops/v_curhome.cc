
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.8: Move the cursor home.
|*
|* Original signature is: v_curhome(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_curhome(qintptr handle)
	{
	if (handle == 0)
		{
		bool erased = _eraseAlphaCursor();
		_cursorX = _cursorY = 0;
		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_curhome(Workstation *)
	{
	v_curhome((qintptr)0);
	}

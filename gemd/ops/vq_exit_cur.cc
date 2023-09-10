#include "debug.h"
#include "screen.h"
#include "vdi.h"


/*****************************************************************************\
|* Opcode 5.2: Exit alpha mode. This clears the screen and disables the
|*             alpha cursor
|*
|* Original signature is:
|*
|*  vq_exit_cur(int16_t handle);
|*
\*****************************************************************************/
void VDI::vq_exit_cur(int16_t handle)
	{
	if (handle != 0)
		{
		WARN("Non-screen devices currently unsupported");
		}
	else
		{
		_alphaMode = false;
		v_clrwk(handle);
		}
	}

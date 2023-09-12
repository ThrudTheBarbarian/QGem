#include "debug.h"
#include "macros.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 2: Close a physical workstation.
|*
|* Original signature is: v_clswk(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_clswk(qintptr handle)
	{
	if (handle != 0)
		{
		WARN("Non-screen devices currently unsupported");
		}
	DELETE(_dpy);
	}

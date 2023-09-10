
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 4: Update a physical workstation.
|*
|* Original signature is: v_updwk(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_updwk(int16_t handle)
	{
	if (handle == 0)
		{
		// nop
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

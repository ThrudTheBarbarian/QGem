
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.22: Clear printer display-list, which isn't supported
|*
|* Original signature is: v_clear_disp_list(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_clear_disp_list(int16_t handle)
	{
	if (handle == 0)
		{
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

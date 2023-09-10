
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.21: Output window to printer, which isn't supported
|*
|* Original signature is: v_output_window(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_output_window(int16_t handle)
	{
	if (handle == 0)
		{
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

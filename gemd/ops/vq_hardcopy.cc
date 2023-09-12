#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.17: dump screen to printer.
|*
|* Original signature is: vq_hardcopy(int16_t handle);
|*
\*****************************************************************************/
void VDI::vq_hardcopy(qintptr handle)
	{
	if (handle == 0)
		{
		// We don't support printing
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

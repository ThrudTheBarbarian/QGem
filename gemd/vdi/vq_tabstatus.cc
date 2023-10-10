
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.16: Get tablet status.
|*
|* Original signature is: vq_tabstatus(int16_t handle);
|*
\*****************************************************************************/
int VDI::vq_tabstatus(qintptr handle)
	{
	if (handle == 0)
		{
		return 0;	// We don't support tablets
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		return 0;
		}
	}

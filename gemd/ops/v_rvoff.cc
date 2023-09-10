
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.14: Disable reverse-video.
|*
|* Original signature is: v_rvoff(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_rvoff(int16_t handle)
	{
	if (handle == 0)
		{
		_reverseVideo = false;
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

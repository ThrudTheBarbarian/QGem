
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.13: Enable reverse-video.
|*
|* Original signature is: v_rvon(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_rvon(int16_t handle)
	{
	if (handle == 0)
		{
		_reverseVideo = true;
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::v_rvon(Workstation *)
	{
	v_rvon((int16_t)0);
	}


#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.20: Form-feed (nop on screen).
|*
|* Original signature is: v_form_adv(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_form_adv(int16_t handle)
	{
	if (handle == 0)
		{
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

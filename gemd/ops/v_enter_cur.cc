
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 5.2: Enter alpha mode. This clears the screen and enables the
|*             alpha cursor
|*
|* Original signature is:
|*
|*  v_enter_cur(int16_t handle);
|*
\*****************************************************************************/
void VDI::v_enter_cur(Workstation *ws)
	{
	_cursorX = 0;
	_cursorY = 0;
	_alphaMode = true;
	v_clrwk(ws);
	}


#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.15: Get the current cursor position.
|*
|* Original signature is: vq_curaddress(int16_t handle,
|*									    int16_t *row,
|*										int16_t *col);
|*
\*****************************************************************************/
void VDI::vq_curaddress(int16_t handle, int16_t& row, int16_t& col)
	{
	if (handle == 0)
		{
		col = _cursorX;
		row = _cursorY;
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

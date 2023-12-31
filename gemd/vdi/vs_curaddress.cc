
#include "clientmsg.h"
#include "debug.h"
#include "screen.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.4: Move the cursor up if possible.
|*
|* Original signature is: v_curup(int16_t handle);
|*
\*****************************************************************************/
void VDI::vs_curaddress(qintptr handle, int16_t row, int16_t col)
	{
	if (handle == 0)
		{
		int maxX = _screen->width()  / _charWidth - 1;
		int maxY = _screen->height() / _charHeight - 1;

		row = (row < 0 )	? 0
			: (row > maxY)	? maxY
			: row;

		col = (col < 0 )	? 0
			: (col > maxX)	? maxX
			: col;

		bool erased = _eraseAlphaCursor();
		_cursorX = col;
		_cursorY = row;
		if (erased)
			_drawAlphaCursor();
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_curaddress(Workstation *, ClientMsg *cm)
	{
	const Payload &p = cm->payload();

	/**************************************************************************\
	|* Get the data out of the message
	\**************************************************************************/
	int16_t row = ntohs(p[0]);
	int16_t col = ntohs(p[1]);
	vs_curaddress(0, row, col);
	}

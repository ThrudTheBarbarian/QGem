//
//  vrq_locator.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/17/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "vdi.h"

/*****************************************************************************\
|*   28 : Request the mouse position by waiting for an event
\*****************************************************************************/
void vrq_locator(int16_t handle, int16_t x, int16_t y,
				 int16_t* x1, int16_t* y1, int16_t *term)
	{
	(void)x;
	(void)y;
	
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Show the mouse cursor
	\*************************************************************************/
	v_dspcur(handle, -1, -1);

	
	/*************************************************************************\
	|* Construct and send the message to add mouse-down to the events-filter
	|* if necessary
	\*************************************************************************/
	GemMsg msg;
	int existing = _gemIoEventFilter();
	int changed  = 0;
	if ((existing & ETYP_MOUSE_BTN) == 0)
		{
		_gemIoSetEventFilter(existing | ETYP_MOUSE_BTN);
		changed = 1;
		}
		
	/*************************************************************************\
	|* Wait for a mouse-down message
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, EVT_MOUSE_DOWN);

	/*************************************************************************\
	|* Hide the mouse cursor
	\*************************************************************************/
	v_rmcur(handle);

	/*************************************************************************\
	|* Set the event filter back to what it was previously
	\*************************************************************************/
	if (changed)
		_gemIoSetEventFilter(existing);

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (x1 != NULL)
		*x1 = ntohs(msg.vec.data[0]);
	if (y1 != NULL)
		*y1 = ntohs(msg.vec.data[1]);
	if (term != NULL)
		{
		int16_t btns =  ntohs(msg.vec.data[2]);
		*term = (btns & 0x1) ? 32
			  : (btns & 0x2) ? 33
			  : 0;
		}
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

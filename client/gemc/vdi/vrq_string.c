//
//  vrq_string.c
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
|*   31 : Request a string input (ie: read the keyboard until return pressed)
|*
|* I can't see any difference between vrq_string and vsm_string from the docs,
|* so just #define'ing it to be the same for now
\*****************************************************************************/
void vrq_string(int16_t handle, int16_t maxLen, int16_t echoMode,
				 int16_t* xy, char* str)
	{
	(void)echoMode;
	(void)xy;
	
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Construct and send the message to add key-events to the events-filter
	|* if necessary
	\*************************************************************************/
	GemMsg msg;
	int existing = _gemIoEventFilter();
	int changed  = 0;
	if ((existing & ETYP_KEYBOARD) == 0)
		{
		_gemIoSetEventFilter(existing | ETYP_KEYBOARD);
		changed = 1;
		}
		
	/*************************************************************************\
	|* Wait for a keyboard message and add it to the string
	\*************************************************************************/
	int done = 0;
	int chars = 0;
	while (!done)
		{
		_gemIoWaitForMessageOfType(&msg, EVT_KEY_PRESS);
		int keycode = ntohs(msg.vec.data[0]);
		if (keycode == KY_RETURN)
			done = 1;
		else if ((keycode == KY_BACKSPACE) && (chars > 0))
			chars --;
		else if ((keycode >= ' ') && (keycode <= '~'))
			{
			str[chars++] = keycode & 0xFF;
			}
		if (chars == maxLen)
			done = 1;
		}
	str[chars] = '\0';
	
	/*************************************************************************\
	|* Set the event filter back to what it was previously
	\*************************************************************************/
	if (changed)
		_gemIoSetEventFilter(existing);		
	}

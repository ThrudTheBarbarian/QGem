//
//  v_curtext.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.12 : Output text to the current cursor position
\*****************************************************************************/
void v_curtext(int16_t handle, const char *str)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_CURTEXT);
	_gemMsgAppendData(&msg, (uint8_t *)str, (int)(strlen(str)+1));
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

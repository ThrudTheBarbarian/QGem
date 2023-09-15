//
//  v_gtext.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   8 : Draw graphic text
\*****************************************************************************/
void v_gtext(int16_t handle, int16_t x, int16_t y, char* name)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* If name is empty or null, just return
	\*************************************************************************/
	if (name == NULL)
		return;
	
	int16_t len = (int16_t)strlen(name);
	if (len == 0)
		return;
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_GTEXT);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemMsgAppendData(&msg, (uint8_t *)name, len+1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

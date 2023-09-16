//
//  v_justified.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|* 11.10: Draw justified text. Note that in this implementation, the hints for
|*        spacing are silently ignored
\*****************************************************************************/
void v_justified(int16_t handle, int16_t x, int16_t y, int16_t length,
				 int16_t wordSpacing, int16_t charSpacing,
				 const char *str)
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
	if (str == NULL)
		return;
	
	int16_t len = (int16_t)strlen(str);
	if (len == 0)
		return;
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_JUSTIFIED);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemMsgAppend(&msg, &length, 1);
	_gemMsgAppendData(&msg, (uint8_t *)str, len+1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

//
//  v_circle.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.4: Fill a circle				[type=4] [pxy=x,y,r]
\*****************************************************************************/
void v_circle(int16_t handle, int16_t x, int16_t y, int16_t radius)
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
	int16_t info[] = {3, x, y, radius};
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_CIRCLE);
	_gemMsgAppend(&msg, info, sizeof(info)/sizeof(int16_t));
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

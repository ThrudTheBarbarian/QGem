//
//  v_ellipse.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.5: Fill an ellipse			[type=5] [pxy=x,y,rx,ry]
\*****************************************************************************/
void v_ellipse(int16_t handle, int16_t x, int16_t y, int16_t rx, int16_t ry)
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
	int16_t info[] = {4, x, y, rx, ry};
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_ELLIPSE);
	_gemMsgAppend(&msg, info, sizeof(info)/sizeof(int16_t));
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

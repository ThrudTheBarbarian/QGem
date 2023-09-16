//
//  v_rbox.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.8: Draw a rounded rect
\*****************************************************************************/
void v_rbox(int16_t handle, int16_t*pts)
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
	int16_t numPts = 4;
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_RBOX);
	_gemMsgAppend(&msg, pts, 4);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

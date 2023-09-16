//
//  v_rfbox.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.9: Fill a rounded rect		[type=9] [pxy=x0,y0,x1,y1]
\*****************************************************************************/
void v_rfbox(int16_t handle, int16_t*pts)
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
	_gemMsgInit(&msg, MSG_V_RFBOX);
	_gemMsgAppend(&msg, &numPts, 1);
	_gemMsgAppend(&msg, pts, 4);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

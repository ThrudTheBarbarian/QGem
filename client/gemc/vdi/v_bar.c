//
//  v_bar.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   11.1	Fill a rectangle		[type=1] [pxy=x0,y0,x1,y1
\*****************************************************************************/
void v_bar(int16_t handle, int16_t*pts)
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
	_gemMsgInit(&msg, MSG_V_BAR);
	_gemMsgAppend(&msg, &numPts, 1);
	_gemMsgAppend(&msg, pts, 4);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

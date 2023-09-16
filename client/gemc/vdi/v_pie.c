//
//  v_pie.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.3: Fill an arc				[type=2] [pxy=x,y,r,begin,end]
\*****************************************************************************/
void v_pie(int16_t handle, int16_t x, int16_t y, int16_t radius,
		   int16_t beginAngle, int16_t endAngle)
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
	int16_t info[] = {5, x, y, radius, beginAngle, endAngle};
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_PIE);
	_gemMsgAppend(&msg, info, sizeof(info)/sizeof(int16_t));
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

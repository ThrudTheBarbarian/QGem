//
//  v_ellarc.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  11.6: Draw an elliptical arc
\*****************************************************************************/
void v_ellarc(int16_t handle, int16_t x, int16_t y, int16_t xr, int16_t yr,
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
	int16_t info[] = {x, y, xr, yr, beginAngle, endAngle};
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_ELLARC);
	_gemMsgAppend(&msg, info, sizeof(info)/sizeof(int16_t));
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

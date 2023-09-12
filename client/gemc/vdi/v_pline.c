//
//  v_pline.c
//  gemc
//
//  Created by Simon Gornall on 9/11/23.
//  Created by ThrudTheBarbarian on 9/11/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*    6  : Draw a poly-line, with at least one point
\*****************************************************************************/
void v_pline(int16_t handle, int16_t numPts, int16_t*pts)
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
	_gemMsgInit(&msg, MSG_V_PLINE);
	_gemMsgAppend(&msg, &numPts, 1);
	_gemMsgAppend(&msg, pts, numPts*2);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

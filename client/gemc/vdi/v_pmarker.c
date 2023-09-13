//
//  v_pmarker.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/13/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*    7  : Draw a poly-marker, with at least one point
\*****************************************************************************/
void v_pmarker(int16_t handle, int16_t numPts, int16_t*pts)
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
	_gemMsgInit(&msg, MSG_V_PMARKER);
	_gemMsgAppend(&msg, &numPts, 1);
	_gemMsgAppend(&msg, pts, numPts*2);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

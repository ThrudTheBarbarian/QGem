//
//  vsl_type.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/11/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   15  : Set the line-type (ie: dash-pattern)
\*****************************************************************************/
void vsl_type(int16_t handle, int16_t which)
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
	_gemMsgInit(&msg, MSG_VSL_TYPE);
	_gemMsgAppend(&msg, &which, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

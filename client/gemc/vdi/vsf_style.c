//
//  vsf_style.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   24  : Set the fill pattern index
\*****************************************************************************/
int16_t vsf_interior(int16_t handle, int16_t which)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return 0;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VSF_STYLE);
	_gemMsgAppend(&msg, &which, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return which;
	}

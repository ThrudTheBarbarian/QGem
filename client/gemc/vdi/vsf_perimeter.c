//
//  vsf_perimeter.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/15/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  104  : Set whether to draw a perimeter around a fill
\*****************************************************************************/
int16_t vsf_perimeter(int16_t handle, int16_t enable)
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
	_gemMsgInit(&msg, MSG_VSF_PERIMETER);
	_gemMsgAppend(&msg, &enable, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return enable;
	}

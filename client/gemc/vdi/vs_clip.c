//
//  vs_clip.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/11/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   129  : Set the clip rectangle
|*
|* pxy contains (in order) {left, top, right, bottom}
\*****************************************************************************/
void vs_clip(int16_t handle, int16_t enable, int16_t* pxy)
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
	_gemMsgInit(&msg, MSG_VS_CLIP);
	_gemMsgAppend(&msg, &enable, 1);
	_gemMsgAppend(&msg, pxy, 4);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

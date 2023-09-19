//
//  v_contourfill.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/16/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  103: Flood-fill from a seed position
\*****************************************************************************/
void v_contourfill(int16_t handle, int16_t x, int16_t y, int16_t index)
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
	_gemMsgInit(&msg, MSG_V_CONTOURFILL);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemMsgAppend(&msg, &index, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

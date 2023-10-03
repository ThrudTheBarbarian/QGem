//
//  vqin_mode.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/3/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  115  : Query the input-mode
\*****************************************************************************/
void vqin_mode(int16_t handle, int16_t device, int16_t* mode)
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
	_gemMsgInit(&msg, MSG_VQIN_MODE);
	_gemMsgAppend(&msg, &device, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQIN_MODE));

	/*************************************************************************\
	|* Retrieve the mode
	\*************************************************************************/
	if (mode != NULL)
		*mode = ntohs(msg.vec.data[0]);
				
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

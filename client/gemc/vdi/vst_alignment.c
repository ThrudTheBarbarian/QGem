//
//  vst_alignment.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/13/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   39  : Set the text-alignment
\*****************************************************************************/
void vst_alignment(int16_t handle, int16_t hIn,   int16_t vIn,
								   int16_t *hOut, int16_t *vOut)
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
	_gemMsgInit(&msg, MSG_VST_ALIGNMENT);
	_gemMsgAppend(&msg, &hIn, 1);
	_gemMsgAppend(&msg, &vIn, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VST_ALIGNMENT));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (vOut != NULL)
		*vOut = msg.vec.data[0];
		
	if (hOut != NULL)
		*hOut = msg.vec.data[1];
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

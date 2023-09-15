//
//  vst_height.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/13/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   12  : Set the text-font height in pixels
\*****************************************************************************/
void vst_height(int16_t handle, int16_t height,
				int16_t* charWidth, int16_t* charHeight,
				int16_t* cellWidth, int16_t* cellHeight)
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
	_gemMsgInit(&msg, MSG_VST_HEIGHT);
	_gemMsgAppend(&msg, &height, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VST_HEIGHT));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (charWidth != NULL)
		*charWidth = ntohs(msg.vec.data[0]);
		
	if (charHeight != NULL)
		*charHeight = ntohs(msg.vec.data[1]);
		
	if (cellWidth != NULL)
		*cellWidth = ntohs(msg.vec.data[2]);
		
	if (cellHeight != NULL)
		*cellHeight = ntohs(msg.vec.data[3]);
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

//
//  vst_point.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  107  : Set the text-font height in points
\*****************************************************************************/
void vst_point(int16_t handle, int16_t height,
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
	_gemMsgInit(&msg, MSG_VST_POINT);
	_gemMsgAppend(&msg, &height, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VST_POINT));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (charWidth != NULL)
		*charWidth = msg.vec.data[0];
		
	if (charHeight != NULL)
		*charHeight = msg.vec.data[1];
		
	if (cellWidth != NULL)
		*cellWidth = msg.vec.data[2];
		
	if (cellHeight != NULL)
		*cellHeight = msg.vec.data[3];
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

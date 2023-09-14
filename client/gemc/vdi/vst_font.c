//
//  vst_font.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   21  : Set the text font
\*****************************************************************************/
int vst_font(int16_t handle, int16_t idx)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return -1;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VST_FONT);
	_gemMsgAppend(&msg, &idx, 1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VST_FONT));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	int fontId = msg.vec.data[0];
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return fontId;
	}

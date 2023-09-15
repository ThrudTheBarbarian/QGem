//
//  vst_load_fonts.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  119  : Tell the server to load up its fonts
\*****************************************************************************/
int vst_load_fonts(int16_t handle, int16_t dummy)
	{
	(void)dummy;
	
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
	_gemMsgInit(&msg, MSG_VST_LOAD_FONTS);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VST_LOAD_FONTS));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	int maxFontId = ntohs(msg.vec.data[0]);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return maxFontId;
	}

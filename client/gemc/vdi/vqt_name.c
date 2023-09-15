//
//  vqt_name.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   130 : Get the name of a font at an index. Name can be up to 32 chars
\*****************************************************************************/
int16_t vqt_name(int16_t handle, int16_t fontId, char* name)
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
	_gemMsgInit(&msg, MSG_VQT_NAME);
	_gemMsgAppend(&msg, &fontId, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQT_NAME));

	/*************************************************************************\
	|* Actual font-id is first
	\*************************************************************************/
	fontId = ntohs(msg.vec.data[0]);
	
	/*************************************************************************\
	|* Length of string is next
	\*************************************************************************/
	int bytes		= ntohs(msg.vec.data[1]);
	int needByte	= bytes & 1;
	bytes 			= bytes & ~1;
	
	/*************************************************************************\
	|* Then copy over the string itself
	\*************************************************************************/
	memcpy(name, &(msg.vec.data[2]), bytes);
	if (needByte != 0)
		{
		int16_t value = msg.vec.data[msg.vec.length-1];
		name[bytes] = value & 0xFF;
		}
	if (bytes < 31)
		name[bytes+1] = '\0';
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return fontId;
	}

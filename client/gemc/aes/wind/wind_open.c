//
//  wind_open.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/26/23.
//

#include "gem.h"

/*****************************************************************************\
|*  6610 : wind_open()
|*		 : Open a window to the specified size
\*****************************************************************************/
int16_t wind_open(int16_t handle, int16_t x, int16_t y, int16_t w, int16_t h)
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
	_gemMsgInit(&msg, MSG_AES_WIND_OPEN);
	_gemMsgAppend(&msg, &handle, 1);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemMsgAppend(&msg, &w, 1);
	_gemMsgAppend(&msg, &h, 1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_WIND_OPEN));
	int16_t success = ntohs(msg.vec.data[0]);

	return success;
	}

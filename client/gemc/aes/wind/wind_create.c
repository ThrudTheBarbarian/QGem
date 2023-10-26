//
//  wind_create.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/26/23.
//

#include "gem.h"

/*****************************************************************************\
|*  6604 : wind_create()
|*		 : Reserve a window structure on the server
\*****************************************************************************/
int16_t wind_create(int16_t kind, int16_t x, int16_t y, int16_t w, int16_t h)
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
	_gemMsgInit(&msg, MSG_AES_WIND_CREATE);
	_gemMsgAppend(&msg, &kind, 1);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemMsgAppend(&msg, &w, 1);
	_gemMsgAppend(&msg, &h, 1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_WIND_CREATE));
	int16_t windowHandle = ntohs(msg.vec.data[0]);

	return windowHandle;
	}

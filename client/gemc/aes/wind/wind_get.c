//
//  wind_get.c
//  gemc
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#include "gem.h"


/*****************************************************************************\
|*  6608 : wind_get()
|*		 : Obtain various pieces of info about a window, and return in the
|*       : gw* variables
\*****************************************************************************/
int16_t wind_get(int16_t windowId, int16_t sub,
				 int16_t* gw1, int16_t* gw2, int16_t* gw3, int16_t* gw4)
	{
	int success = 0;
	
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
	_gemMsgInit(&msg, MSG_AES_WIND_GET);
	_gemMsgAppend(&msg, &windowId, 1);
	_gemMsgAppend(&msg, &sub, 1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_WIND_GET));
	
	if (msg.vec.length == 5)
		{
		if (ntohs(msg.vec.data[0]) != 0)
			{
			success = 1;
			if (gw1)
				*gw1 = ntohs(msg.vec.data[1]);
			if (gw2)
				*gw2 = ntohs(msg.vec.data[2]);
			if (gw3)
				*gw3 = ntohs(msg.vec.data[3]);
			if (gw4)
				*gw4 = ntohs(msg.vec.data[4]);
			}
		}
		
	return success;
	}

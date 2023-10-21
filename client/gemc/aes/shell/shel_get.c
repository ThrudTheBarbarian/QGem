//
//  shel_get.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/20/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "map.h"

/*****************************************************************************\
|*  7902 : shel_get()
|*		 : obtains the GEM desktop environment information
\*****************************************************************************/
int16_t shel_get(char *info, uint16_t maxBytes)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			{
			WARN("Cannot connect to GEMd service");
			return 0;
			}
	
	/*************************************************************************\
	|* Check to see if the variable is valid
	\*************************************************************************/
	if (info == NULL)
		{
		WARN("Passed NULL variable to shel_get");
		return 0;
		}
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_GET);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_SHEL_GET));

	/*************************************************************************\
	|* Retrieve the string value, If we find the file, the full path will be
	|* written into 'name', and we return 1. Otherwise we write '\0' into
	|* 'name' and return 0
	\*************************************************************************/
	uint32_t length = _gemMsgDataLength(&msg, 0);
	int16_t ok 		= 0;
	if (length == 0)
		{
		WARN("shel_get got zero-length string ?");
		info[0] = '\0';
		}
	else
		{
		uint8_t buf[length+1];
		_gemMsgFetchData(&msg, 2, buf, length);
		int bytesToXfer = MIN(length, maxBytes-1);
		memcpy(info, buf, bytesToXfer);
		info[bytesToXfer] = '\0';
		ok = 1;
		}
		
	return ok;
	}


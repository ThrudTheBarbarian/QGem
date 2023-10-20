//
//  shel_envrn.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/19/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  7900 : shel_envrn()
|*		 : obtains the value of an environment variable
\*****************************************************************************/
int16_t shel_envrn(int8_t **value, int8_t *name)
	{
	const char * str = (const char *)name;
	
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			{
			WARN("Cannot connect to GEMd service");
			if (value != NULL)
				*value = NULL;
			return 1;
			}
	
	/*************************************************************************\
	|* Check to see if the variable is valid
	\*************************************************************************/
	if (name == NULL)
		{
		WARN("Passed NULL variable to shel_envrn");
		if (value != NULL)
			*value = NULL;
		return 1;
		}
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_ENVRN);
	_gemMsgAppendData(&msg, (uint8_t *)str, (uint32_t)strlen(str)+1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_SHEL_ENVRN));

	/*************************************************************************\
	|* Retrieve the string value, if it's length is 0, we return "", and if
	|* it's not found, we return NULL
	\*************************************************************************/
	int16_t found = ntohs(msg.vec.data[0]);
	if (found != 0)
		{
		uint32_t length = _gemMsgDataLength(&msg, 2);
		if (length == 0)
			{
			if (value)
				*value[0] = '\0';
			}
		else
			_gemMsgFetchData(&msg, 2, (uint8_t *)(*value), length);
		}
	else
		{
		if (value)
			*value = NULL;
		}
	return 1;
	}


//
//  shel_find.c
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
|*  7901 : shel_find()
|*		 : Search for a file in a variety of system dirs. Note that 'name'
|* 		 : on this system should be PATH_MAX bytes long, the original Atari
|*       : suggestion was 128, which is not sufficient
|*
|* 		 : Returns 0 if the file cannot be found
\*****************************************************************************/
int16_t shel_find(char *name)
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
	if (name == NULL)
		{
		WARN("Passed NULL variable to shel_find");
		return 0;
		}
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_FIND);
	_gemMsgAppendData(&msg, (uint8_t *)name, (int)strlen(name)+1);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_SHEL_FIND));

	/*************************************************************************\
	|* Retrieve the string value, If we find the file, the full path will be
	|* written into 'name', and we return 1. Otherwise we write '\0' into
	|* 'name' and return 0
	\*************************************************************************/
	int16_t found = ntohs(msg.vec.data[0]);
	if (found != 0)
		{
		uint32_t length = _gemMsgDataLength(&msg, 1);
		if (length == 0)
			{
			WARN("shel_find got zero-length string, but found the file ?");
			if (name)
				name[0] = '\0';
			found = 0;
			}
		else
			{
			if (name)
				_gemMsgFetchData(&msg, 3, (uint8_t *)name, length);
			}
		}
	else
		{
		if (name)
			name[0] = '\0';
		}
		
	return found;
	}


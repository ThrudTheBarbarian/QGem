//
//  shel_read.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/20/23.
//

#include <stdio.h>

#include "gem.h"
#include "macros.h"

/*****************************************************************************\
|*  7906 : shel_read()
|*		 : Get the commandline parameters for an application launched with
|*       : shel_write()
|*
|* Note that both cmd and args ought to be of length at least PATH_MAX
\*****************************************************************************/
int16_t shel_read(char *cmd, char *args)
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
	if ((cmd == NULL) && (args == NULL))
		{
		WARN("Passed NULL variables to shel_read");
		return 0;
		}
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_READ);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_SHEL_READ));

	/*************************************************************************\
	|* Retrieve the command
	\*************************************************************************/
	uint32_t length = _gemMsgDataLength(&msg, 0);
	int16_t ok 		= 0;
	int nextIdx		= 0;
	
	if (length == 0)
		{
		if (cmd)
			*cmd = '\0';
		if (args)
			*args = '\0';
		}
	else
		{
		uint8_t buf[length+1];
		nextIdx = _gemMsgFetchData(&msg, 2, buf, length);
		if (cmd)
			{
			memcpy(cmd, buf, length);
			ok ++;
			}
		}

	/*************************************************************************\
	|* Retrieve the arguments
	\*************************************************************************/
	if (nextIdx > 0)
		{
		length = _gemMsgDataLength(&msg, nextIdx);
		
		if (length == 0)
			{
			if (args)
				*args = '\0';
			}
		else
			{
			uint8_t buf[length+1];
			_gemMsgFetchData(&msg, nextIdx+2, buf, length);
			if (args)
				{
				memcpy(args, buf, length);
				ok ++;
				}
			}
		}
		
	return (ok > 0);
	}


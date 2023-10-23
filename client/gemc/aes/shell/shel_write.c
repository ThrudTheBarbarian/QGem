//
//  shel_write.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/22/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "map.h"

/*****************************************************************************\
|*  7908 : shel_write()
|*		 : Interface to executing an app, or sending messages to other apps
\*****************************************************************************/
int16_t shel_write(int16_t doex,
				   int16_t isgr,
				   int16_t iscr,
				   char *  cmd,
				   char *  args)
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
	if (cmd == NULL)
		{
		WARN("Passed NULL variable to shel_write");
		return 0;
		}
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_WRITE);
	_gemMsgAppend(&msg, &doex, 1);
	_gemMsgAppend(&msg, &isgr, 1);
	_gemMsgAppend(&msg, &iscr, 1);
	_gemMsgAppendData(&msg, (uint8_t *)cmd, (int)strlen(cmd)+1);
	if (args == NULL)
		args = "";
	_gemMsgAppendData(&msg, (uint8_t *)args, (int)strlen(args)+1);
	_gemIoWrite(&msg);

	return 1;
	}

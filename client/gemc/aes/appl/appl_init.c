//
//  appl_init.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  6007 : appl_init()
|*		 : Registers the application under AES
\*****************************************************************************/
int16_t appl_init(void)
	{
	int16_t appId = -1;
	
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return appId;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_APPL_INIT);

	/*************************************************************************\
	|* If we've been launched with shel_write, then we ought to have an
	|* environment variable set to our UUID on the server, if so, pass it
	|* through
	\*************************************************************************/
	char * appUUID = getenv("GEM_APP_ID");
	if (appUUID != NULL)
		_gemMsgAppendData(&msg, (uint8_t*)appUUID, (int)strlen(appUUID));
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_APPL_INIT));

	/*************************************************************************\
	|* Get back an application handle
	\*************************************************************************/
	appId = ntohs(msg.vec.data[0]);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
		
	/*************************************************************************\
	|* Return the app-id
	\*************************************************************************/
	return appId;
	}

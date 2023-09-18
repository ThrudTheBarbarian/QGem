//
//  vqm_attributes.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/17/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   36  : Get the current marker attributes
|*         returns: type, pen, mode, height (pix)
\*****************************************************************************/
void vqm_attributes(int16_t handle, int16_t*settings)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VQM_ATTRIBUTES);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQM_ATTRIBUTES));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (settings != NULL)
		{
		settings[0] = ntohs(msg.vec.data[0]);
		settings[1] = ntohs(msg.vec.data[1]);
		settings[2] = ntohs(msg.vec.data[2]);
		settings[3] = ntohs(msg.vec.data[3]);
		}
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

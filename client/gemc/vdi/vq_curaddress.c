//
//  vq_curaddress.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.15 : Get the position of the cursor on the screen
\*****************************************************************************/
void vq_curaddress(int16_t handle, int16_t* row, int16_t* col)
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
	_gemMsgInit(&msg, MSG_VQ_CURADDRESS);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY_OFFSET+ MSG_VQ_CURADDRESS);

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (row != NULL)
		*row = msg.vec.data[0];
		
	if (col != NULL)
		*col = msg.vec.data[1];
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

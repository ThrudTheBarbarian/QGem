//
//  vq_chcells.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.1 : Get the number of character cells on the alpha screen
\*****************************************************************************/
void vq_chcells(int16_t handle, int16_t* rows, int16_t* cols)
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
	_gemMsgInit(&msg, MSG_VQ_CHCELLS);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY_OFFSET+ MSG_VQ_CHCELLS);

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (rows != NULL)
		*rows = msg.vec.data[0];
		
	if (cols != NULL)
		*cols = msg.vec.data[1];
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

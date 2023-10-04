//
//  vqt_width.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/3/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  117  : Get character width
\*****************************************************************************/
void vqt_width(int16_t handle,
			   char c,
			   int16_t *cellWidth,
			   int16_t *leftDelta,
			   int16_t * rightDelta)
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
	int16_t letter = c;

	GemMsg msg;
	_gemMsgInit(&msg, MSG_VQT_WIDTH);
	_gemMsgAppend(&msg, &letter, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQT_WIDTH));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (cellWidth != NULL)
		*cellWidth = ntohs(msg.vec.data[0]);
	if (leftDelta != NULL)
		*leftDelta = ntohs(msg.vec.data[1]);
	if (rightDelta != NULL)
		*rightDelta = ntohs(msg.vec.data[2]);
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

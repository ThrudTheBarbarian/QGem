//
//  vq_extnd.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/18/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|* 102   : Enquire about extended information
\*****************************************************************************/
void vq_extnd(int16_t handle, int16_t extend, int16_t *workOut)
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
	_gemMsgInit(&msg, MSG_VQ_EXTND);
	_gemMsgAppend(&msg, &extend, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQ_EXTND));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (workOut != NULL)
		{
		int words = MIN(57, msg.vec.length);
		memcpy(workOut, msg.vec.data, words * sizeof(int16_t));
		for (int i=0; i<words; i++)
			workOut[i] = ntohs(workOut[i]);
		}
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

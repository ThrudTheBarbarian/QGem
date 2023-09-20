//
//  vq_pixrgb.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/19/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  60  : Get a pixel's rgb.
\*****************************************************************************/
void vq_pixrgb(int16_t handle, int16_t x, int16_t y, int16_t* rgb)
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
	_gemMsgInit(&msg, MSG_VQ_PIXRGB);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQ_PIXRGB));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (rgb != NULL)
		{
		rgb[0] = ntohs(msg.vec.data[0]);
		rgb[1] = ntohs(msg.vec.data[1]);
		rgb[2] = ntohs(msg.vec.data[2]);
		}

	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

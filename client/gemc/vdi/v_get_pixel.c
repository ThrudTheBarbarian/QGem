//
//  v_get_pixel.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/19/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  105  : Get a pixel. This is compatible with the old GEM interface
|*        	but only works to 16-bit colour resolution. Use vq_pixrgb
|*			(opcode 60) to get 24-bit RGB.
|*
|* Pixel values are returned as RRRRRggggggBBBBB, with idx always being -1
\*****************************************************************************/
void v_get_pixel(int16_t handle, int16_t x, int16_t y,
				 int16_t* pixel, int16_t *idx)
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
	_gemMsgInit(&msg, MSG_V_GET_PIXEL);
	_gemMsgAppend(&msg, &x, 1);
	_gemMsgAppend(&msg, &y, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_V_GET_PIXEL));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (pixel != NULL)
		*pixel = ntohs(msg.vec.data[0]);
		
	if (idx != NULL)
		*idx = ntohs(msg.vec.data[1]);
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

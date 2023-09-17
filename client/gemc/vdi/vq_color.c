//
//  vq_color.c
//  gemc
//
//  Created by Simon Gornall on 9/16/23.
//

#include <stdio.h>
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
|*   26 : Get the RGB values of a colour index
\*****************************************************************************/
void vq_color(int16_t handle, int16_t idx, int16_t flag, int16_t* rgb)
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
	_gemMsgInit(&msg, MSG_VQ_COLOR);
	_gemMsgAppend(&msg, &idx, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQ_COLOR));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (rgb != NULL)
		{
		rgb[0] = ntohs(msg.vec.data[0]);
		rgb[1] = ntohs(msg.vec.data[0]);
		rgb[2] = ntohs(msg.vec.data[0]);
		}
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

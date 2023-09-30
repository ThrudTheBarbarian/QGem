//
//  vq_colours.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/26/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   2026 : Get the RGB values of the colour palette
\*****************************************************************************/
void vq_colours(int16_t handle, int16_t* rgb)
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
	_gemMsgInit(&msg, MSG_VQ_COLOURS);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQ_COLOURS));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (rgb != NULL)
		for (int i=0; i<256 * 3; i++)
			rgb[i] = ntohs(msg.vec.data[i]);
	else
		WARN("Need to supply a pointer for the colour palette");
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

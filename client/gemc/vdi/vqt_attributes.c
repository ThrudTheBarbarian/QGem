//
//  vqt_attributes.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/17/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   38  : Get the current text attributes
|*         returns: font, colour, angle, halign, valign, mode, charW, charH,
|*                  cellW, cellH
\*****************************************************************************/
void vqt_attributes(int16_t handle, int16_t*settings)
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
	_gemMsgInit(&msg, MSG_VQT_ATTRIBUTES);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQT_ATTRIBUTES));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (settings != NULL)
		for (int i=0; i<10; i++)
			settings[i] = ntohs(msg.vec.data[i]);
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

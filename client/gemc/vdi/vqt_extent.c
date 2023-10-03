//
//  vqt_extent.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/3/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   38  : Get the current text extents
\*****************************************************************************/
void vqt_extent(int16_t handle, char* str, int16_t *pxy8)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;

	/*************************************************************************\
	|* If str is empty or null, just return
	\*************************************************************************/
	if (str == NULL)
		return;
	
	int16_t len = (int16_t)strlen(str);
	if (len == 0)
		return;

	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VQT_EXTENT);
	_gemMsgAppendData(&msg, (uint8_t *)str, len+1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQT_EXTENT));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (pxy8 != NULL)
		for (int i=0; i<8; i++)
			pxy8[i] = ntohs(msg.vec.data[i]);
		
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

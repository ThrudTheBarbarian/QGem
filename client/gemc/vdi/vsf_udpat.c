//
//  vsf_udpat.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/2/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  112  : Set a user-defined pattern fill
\*****************************************************************************/
void vsf_udpat(int16_t handle, int16_t *pat, int16_t planes)
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
	_gemMsgInit(&msg, MSG_VSF_UDPAT);
	_gemMsgAppend(&msg, &planes, 1);
	_gemMsgAppend(&msg, pat, 16*planes);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

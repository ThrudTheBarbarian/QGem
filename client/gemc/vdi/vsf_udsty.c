//
//  vsf_udsty.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/3/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  112  : Set a user-defined line-style
\*****************************************************************************/
void vsf_udsty(int16_t handle, int16_t pat)
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
	_gemMsgInit(&msg, MSG_VSF_UDSTY);
	_gemMsgAppend(&msg, &pat, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

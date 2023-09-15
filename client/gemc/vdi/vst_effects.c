//
//  vst_effects.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/14/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  106  : Set the text-effects
\*****************************************************************************/
int vst_effects(int16_t handle, int16_t effect)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return 0;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VST_EFFECTS);
	_gemMsgAppend(&msg, &effect, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	
	return effect & TXT_MASK;
	}

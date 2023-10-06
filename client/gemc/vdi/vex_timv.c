//
//  vex_timv.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/3/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  Static variables
\*****************************************************************************/
static GemTimerFunction		_currentFn = NULL;

/*****************************************************************************\
|*  118  : Register a timer function
\*****************************************************************************/
void vex_timv(int16_t handle,
			  GemTimerFunction newFunc,
			  GemTimerFunction *oldFunc,
			  int16_t *mpt)
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
	int16_t enable = (newFunc == NULL) ? 0 : 1;
	
	_gemMsgInit(&msg, MSG_VEX_TIMV);
	_gemMsgAppend(&msg, &enable, 1);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VEX_TIMV));

	if (mpt)
		*mpt		= ntohs(msg.vec.data[0]);
	if (oldFunc)
		*oldFunc	= _currentFn;
	_currentFn 	= newFunc;

	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

/*****************************************************************************\
|* Called by the message-receiver code while the timer vector is enabled
\*****************************************************************************/
void _gemProcessTimerEvent(void)
	{
	if (_currentFn != NULL)
		_currentFn();
	}

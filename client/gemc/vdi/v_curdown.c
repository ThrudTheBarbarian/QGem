//
//  v_curdown.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.5 : Cursor down if possible
\*****************************************************************************/
void v_curdown(int handle)
	{
	(void)handle;
	
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
	_gemMsgInit(&msg, MSG_V_CURDOWN);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

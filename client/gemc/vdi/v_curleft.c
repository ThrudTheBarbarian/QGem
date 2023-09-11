//
//  v_curleft.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.7 : Cursor left if possible
\*****************************************************************************/
void v_curleft(int handle)
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
	_gemMsgInit(&msg, MSG_V_CURLEFT);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

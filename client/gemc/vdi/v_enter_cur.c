//
//  v_enter_cur.c
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.3 : Enter alpha mode
\*****************************************************************************/
void v_enter_cur(int handle)
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
	_gemMsgInit(&msg, MSG_V_ENTER_CUR);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

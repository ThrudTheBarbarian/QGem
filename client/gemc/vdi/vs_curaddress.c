//
//  vs_curaddress.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*   5.11 : Get the number of character cells on the alpha screen
\*****************************************************************************/
void vs_curadress(int16_t handle, int16_t row, int16_t col)
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
	_gemMsgInit(&msg, MSG_VS_CURADDRESS);
	_gemMsgAppend(&msg, &row, 1);
	_gemMsgAppend(&msg, &col, 1);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

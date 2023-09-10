//
//  v_opnvwk.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|* Initialise the GEM subsystem if necessary, and open a virtual workstation
\*****************************************************************************/
void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut)
	{
	static int16_t inputs[16];


	if (!_gemIoIsConnected())
		_gemIoConnect();
	
	/*************************************************************************\
	|* Initialise values if none were provided
	\*************************************************************************/
	int16_t *arg = workIn;
	if (workIn == NULL)
		{
		for (int i=0; i<16; i++)
			inputs[i] = -1;
		arg = inputs;
		}
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_OPNVWK);
	_gemMsgAppend(&msg, arg, 16);
	_gemIoWrite(&msg);
	
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY_OFFSET+ MSG_V_OPNVWK);

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (workOut != NULL)
		{
		int words = MIN(57, msg.vec.length);
		memcpy(workOut, msg.vec.data, words * sizeof(int16_t));
		}
		
	if ((handle != NULL) && (msg.vec.length > 56))
		*handle = msg.vec.data[57];
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

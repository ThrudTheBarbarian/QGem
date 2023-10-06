//
//  v_opnvwk.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|* 100   : Open a virtual workstation
\*****************************************************************************/
void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut)
	{
	static int16_t inputs[16];

	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
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
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_V_OPNVWK));

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (workOut != NULL)
		{
		int words = MIN(57, msg.vec.length);
		memcpy(workOut, msg.vec.data, words * sizeof(int16_t));
		for (int i=0; i<words; i++)
			workOut[i] = ntohs(workOut[i]);
		}
		
	if ((handle != NULL) && (msg.vec.length > 56))
		*handle = ntohs(msg.vec.data[57]);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);

	/*************************************************************************\
	|* Start receiving events
	\*************************************************************************/
	_gemIoSetEventFilter(ETYP_MOUSE_MOVE|ETYP_MOUSE_BTN);
	
	}

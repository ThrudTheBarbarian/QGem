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
|* Parameters passed back once the virtual workstation is opened. Fill with
|* some default values to start off with
\*****************************************************************************/
static int16_t _wsParam[65] =
	{
	1920, 1080, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,6,14,7,
	16,0,0,0,0
	};

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
	|* Take a copy. Note that this copies the extra information passed by the
	|* server, to whit:
	|*
	|* 56 : The handle id
	|* 57 : The char width
	|* 58 : The char height
	|* 59 : The box width
	|* 60 : The box height
	\*************************************************************************/
	memcpy(_wsParam, msg.vec.data, msg.vec.length * sizeof(int16_t));
	for (int i=0; i<msg.vec.length; i++)
		_wsParam[i] = ntohs(_wsParam[i]);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);

	/*************************************************************************\
	|* Start receiving events
	\*************************************************************************/
	_gemIoSetEventFilter(ETYP_MOUSE_MOVE|ETYP_MOUSE_BTN);
	
	}

/*****************************************************************************\
|* Return the parameters from v_opnvwk, used by the AES
\*****************************************************************************/
int16_t * v_opnvwkParams(void)
	{
	return _wsParam;
	}

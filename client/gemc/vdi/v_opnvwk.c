//
//  v_opnvwk.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"

/*****************************************************************************\
|* Initialise the GEM subsystem if necessary, and open a virtual workstation
\*****************************************************************************/
void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut)
	{
	if (!_gemIoIsConnected())
		_gemIoConnect();
	
	GemMsg msg;
	_gemMsgInit(&msg, MSG_V_OPNVWK);
	_gemMsgAppend(&msg, workIn, 16);
	_gemIoWrite(&msg);
	}

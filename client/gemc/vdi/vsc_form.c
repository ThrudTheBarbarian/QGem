//
//  vsc_form.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/2/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  111  : Set the mouse form
|*
|* pform[0]	X-coordinate, and
|* pform[1]	Y-coordinate of hot-spot
|* pform[2]	Number of planes, must be 1
|* pform[3]	Colour index of background mask (should be 0)
|* pform[4]	Colour index of foreground mask (should be 1)
|* pform[05..20]	Definition of background mask
|* pform[21..36]	Definition of foreground mask
\*****************************************************************************/
void vsc_form(int16_t handle, int16_t *pform)
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
	_gemMsgInit(&msg, MSG_VSC_FORM);
	_gemMsgAppend(&msg, pform, 37);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

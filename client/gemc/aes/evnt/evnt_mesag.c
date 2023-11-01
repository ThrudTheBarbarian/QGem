//
//  evnt_mesag.c
//  gemc
//
//  Created by Simon Gornall on 11/1/23.
//

#include "gem.h"


/*****************************************************************************\
|* Listen on the event pipe and only return when we get a GEM-message type
|* event. We still populate the mouse position events etc.
\*****************************************************************************/
int16_t evnt_mesag(int16_t* msg)
	{
	GemMsg ioMsg;
	_gemIoWaitForMessageRange(&ioMsg, EVT_WM_BASE, EVT_WM_BASE+999);
	
	/*************************************************************************\
	|* Copy the payload information over to the int buffer
	\*************************************************************************/
	if (msg)
		{
		msg[0] = ioMsg.type - EVT_WM_BASE;
		for (int i=0; i<7; i++)
			msg[i+1] = ntohs(ioMsg.vec.data[i]);
		}
	return 1;
	}

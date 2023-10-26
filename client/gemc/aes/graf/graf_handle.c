//
//  graf_handle.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/10/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  7002 : graf_handle()
|*		 : Retrieves the AES physical workstation id and char stats
\*****************************************************************************/
int16_t graf_handle(int16_t *wchr, int16_t *hchr, int16_t *wbox, int16_t *hbox)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return -1;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_GRAF_HANDLE);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_GRAF_HANDLE));

	/*************************************************************************\
	|* Get back the physical workstation handle and character types
	\*************************************************************************/
	int16_t physHandle  = ntohs(msg.vec.data[0]);
	if (wchr) *wchr		= ntohs(msg.vec.data[1]);
	if (hchr) *hchr		= ntohs(msg.vec.data[2]);
	if (wbox) *wbox		= ntohs(msg.vec.data[3]);
	if (hbox) *hbox		= ntohs(msg.vec.data[4]);
	
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
		
	/*************************************************************************\
	|* Return the app-id
	\*************************************************************************/
	return physHandle;
	}

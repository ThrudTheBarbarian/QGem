//
//  shel_put.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/21/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "map.h"

/*****************************************************************************\
|*  7903 : shel_put()
|*		 : stores the GEM desktop environment information
\*****************************************************************************/
int16_t shel_put(char *info, uint16_t maxBytes)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			{
			WARN("Cannot connect to GEMd service");
			return 0;
			}
	
	/*************************************************************************\
	|* Check to see if the variable is valid
	\*************************************************************************/
	if (info == NULL)
		{
		WARN("Passed NULL variable to shel_put");
		return 0;
		}
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_PUT);
	_gemMsgAppendData(&msg, (uint8_t *)info, maxBytes);
	_gemIoWrite(&msg);

	return 1;
	}

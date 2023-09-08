//
//  gemio.h
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef gemio_h
#define gemio_h

#include <stdio.h>
#include "gemmsg.h"

/*****************************************************************************\
|* Enumerate the message-types for going back and forth to the server
\*****************************************************************************/
enum
	{
	MSG_V_OPNVWK	= 100,
	};
	
/*****************************************************************************\
|* Check to see if the connection has been made to the server
\*****************************************************************************/
int _gemIoIsConnected(void);

/*****************************************************************************\
|* Connect to the server
\*****************************************************************************/
int _gemIoConnect(void);

/*****************************************************************************\
|* Write to the socket
\*****************************************************************************/
int _gemIoWrite(GemMsg *msg);

#endif /* gemio_h */

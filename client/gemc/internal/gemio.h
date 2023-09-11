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
	MSG_V_CLRWK			= 3,
	MSG_V_OPNVWK		= 100,
	MSG_VQ_CHCELLS		= 5001,
	MSG_VQ_EXIT_CUR		= 5002,
	MSG_V_ENTER_CUR		= 5003,
	MSG_V_CURUP			= 5004,
	MSG_V_CURDOWN		= 5005,
	MSG_V_CURRIGHT		= 5006,
	MSG_V_CURLEFT		= 5007,
	MSG_V_CURHOME		= 5008,
	MSG_V_EEOS			= 5009,
	MSG_V_EEOL			= 5010,
	MSG_VS_CURADDRESS	= 5011,
	MSG_V_CURTEXT		= 5012,
	MSG_V_RVON			= 5013,
	MSG_V_RVOFF			= 5014,
	MSG_VQ_CURADDRESS	= 5015,
	MSG_REPLY_OFFSET	= 8192,
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

/*****************************************************************************\
|* Read from the socket
\*****************************************************************************/
int _gemIoRead(GemMsg *msg);

/*****************************************************************************\
|* Read from the socket, looking for a specific response
\*****************************************************************************/
int _gemIoWaitForMessageOfType(GemMsg *msg, int16_t type);
	

#endif /* gemio_h */

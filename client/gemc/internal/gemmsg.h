//
//  gemmsg.h
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef gemmsg_h
#define gemmsg_h

#include <stdio.h>
#include "vec.h"

typedef struct
	{
	int16_t 			type;			// Type of the message
	vec_word_t			vec;			// bytes of the message
	} GemMsg;

typedef vec_t(GemMsg)	MsgList;		// Vector of messages, shallow copies

/*****************************************************************************\
|* Initialise a message to send to the server
\*****************************************************************************/
void _gemMsgInit(GemMsg* msg, int16_t type);

/*****************************************************************************\
|* Add words into the message
\*****************************************************************************/
void _gemMsgAppend(GemMsg *msg, int16_t *data, int numWords);

/*****************************************************************************\
|* Prevent memory leaks
\*****************************************************************************/
void _gemMsgDestroy(GemMsg *msg);
	
/*****************************************************************************\
|* Interpret the data as int16_t
\*****************************************************************************/
int16_t _gemMsgData(GemMsg *msg, int idx);
	
/*****************************************************************************\
|* Read a message from the wire
\*****************************************************************************/
int _gemMsgRead(GemMsg *msg, int fd);

#endif /* gemmsg_h */

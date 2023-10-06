//
//  gemmsg.h
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef gemmsg_h
#define gemmsg_h

#include <stdio.h>

#include "gemTypes.h"
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
|* Add data into the message with a preprended length
\*****************************************************************************/
void _gemMsgAppendData(GemMsg *msg, uint8_t *data, uint32_t numBytes);

/*****************************************************************************\
|* Append an MFDB to a message
\*****************************************************************************/
void _gemMsgAppendMfdb(GemMsg *msg, MFDB *mfdb);

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
int _gemMsgRead(GemMsg *msg, int fd, int msecs);

#endif /* gemmsg_h */

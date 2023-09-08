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
	vec_data_t			vec;			// bytes of the message
	int16_t 			checksum;		// checksum of the message
	} GemMsg;

/*****************************************************************************\
|* Initialise a message to send to the server
\*****************************************************************************/
void _gemMsgInit(GemMsg* msg, int16_t type);

/*****************************************************************************\
|* Add words into the message
\*****************************************************************************/
void _gemMsgAppend(GemMsg *msg, int16_t *data, int numWords);

#endif /* gemmsg_h */

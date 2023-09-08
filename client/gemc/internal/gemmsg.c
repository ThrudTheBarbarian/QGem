//
//  gemmsg.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include "gemmsg.h"

/*****************************************************************************\
|* Initialise a message to send to the server
\*****************************************************************************/
void _gemMsgInit(GemMsg* msg, int16_t type)
	{
	if (msg != NULL)
		{
		msg->type 		= type;
		msg->checksum 	= 0;
		vec_init(&(msg->vec));
		}
	}

/*****************************************************************************\
|* Add words into the message
\*****************************************************************************/
void _gemMsgAppend(GemMsg *msg, int16_t *data, int numWords)
	{
	if ((msg != NULL) && (data != NULL))
		{
		for (int i=0; i<numWords; i++)
			{
			msg->checksum += data[i];
			uint16_t val   = (uint16_t) data[i];
			vec_push(&(msg->vec), val & 0xFF);
			vec_push(&(msg->vec), val >> 8);
			}
		}
	}


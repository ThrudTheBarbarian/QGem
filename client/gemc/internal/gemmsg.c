//
//  gemmsg.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include "gemmsg.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
 
/*****************************************************************************\
|* Helper to see how many bytes are available to read
\*****************************************************************************/
static int bytesAvailable(int fd)
	{
	int bytesAvailable = 0;
	if (ioctl(fd, FIONREAD, &bytesAvailable) > 0)
		return bytesAvailable;
	return 0;
	}
 
/*****************************************************************************\
|* Initialise a message to send to the server
\*****************************************************************************/
void _gemMsgInit(GemMsg* msg, int16_t type)
	{
	if (msg != NULL)
		{
		msg->type 		= type;
		msg->checksum 	= 0;
		msg->data		= NULL;
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

/*****************************************************************************\
|* Prevent memory leaks
\*****************************************************************************/
void _gemMsgDestroy(GemMsg *msg)
	{
	vec_deinit(&(msg->vec));
	msg->type 		= 0;
	msg->checksum 	= 0;
	msg->data		= NULL;
	}

/*****************************************************************************\
|* Interpret the data as int16_t
\*****************************************************************************/
int16_t _gemMsgData(GemMsg *msg, int idx)
	{
	idx 			= 2 + idx * 2;		// Type + offset
	uint8_t *data 	= msg->vec.data;
	return (int16_t)(data[idx] + (data[idx+1] * 256));
	}

/*****************************************************************************\
|* Read a message from the wire
\*****************************************************************************/
int _gemMsgRead(GemMsg *msg, int fd)
	{
	int ok = 0;
	
	if (bytesAvailable(fd) > 2)
		{
		/*********************************************************************\
		|* Get the size of the payload, including type and checksum
		\*********************************************************************/
		uint8_t sizebuf[2];
		if (read(fd, sizebuf, 2) == 2)
			{
			int length = sizebuf[0] + sizebuf[1] * 256;

			/*****************************************************************\
			|* Check we can read that many bytes
			\*****************************************************************/
			if (bytesAvailable(fd) >= length)
				{
				vec_init(&(msg->vec));
				vec_reserve(&(msg->vec), length);

				/*************************************************************\
				|* Read in the data and calculate the checksum
				\*************************************************************/
				if (read(fd, msg->vec.data, length) == length)
					{
					uint8_t *data 		= msg->vec.data;
					msg->data			= (int16_t*)(data + 2);
					msg->type 			= (int16_t)(data[0] + data[1] * 256);
					int words			= (length - 2 - 2) / 2;
					int16_t checksum	= 0;
					int16_t streamsum	= _gemMsgData(msg, words);
					for (int i=0; i<words; i++)
						checksum += _gemMsgData(msg, i);
					if (checksum == streamsum)
						ok = 1;
					}
				}
			}
		}
	return ok;
	}

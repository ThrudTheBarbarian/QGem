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
//static int bytesAvailable(int fd)
//	{
//	int bytesAvailable = 0;
//	if (ioctl(fd, FIONREAD, &bytesAvailable) > 0)
//		return bytesAvailable;
//	return 0;
//	}
 
/*****************************************************************************\
|* Initialise a message to send to the server
\*****************************************************************************/
void _gemMsgInit(GemMsg* msg, int16_t type)
	{
	if (msg != NULL)
		{
		msg->type 		= type;
		vec_init(&(msg->vec));
		}
	}

/*****************************************************************************\
|* Add words into the message
\*****************************************************************************/
void _gemMsgAppend(GemMsg *msg, int16_t *data, int numWords)
	{
	if ((msg != NULL) && (data != NULL))
		for (int i=0; i<numWords; i++)
			vec_push(&(msg->vec), *data++);
	}

/*****************************************************************************\
|* Prevent memory leaks
\*****************************************************************************/
void _gemMsgDestroy(GemMsg *msg)
	{
	vec_deinit(&(msg->vec));
	msg->type 		= 0;
	}

/*****************************************************************************\
|* Read a message from the wire. This will block
\*****************************************************************************/
int _gemMsgRead(GemMsg *msg, int fd)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* Get the size of the payload in words, including type
	\*************************************************************************/
	int16_t length;
	if (read(fd, &length, 2) == 2)
		{
		length = ntohs(length);
		/*********************************************************************\
		|* Read the type
		\*********************************************************************/
		if (read(fd, &(msg->type), 2) == 2)
			{
			msg->type = ntohs(msg->type);
			length --;
			
			/*****************************************************************\
			|* Allocate space for the message content
			\*****************************************************************/
			vec_init(&(msg->vec));
			vec_reserve(&(msg->vec), length);

			/*****************************************************************\
			|* Read in the data
			\*****************************************************************/
			if (read(fd, msg->vec.data, length*2) == length*2)
				{
				int16_t *ptr = (int16_t*)(msg->vec.data);
				for (int i=0; i<length; i++)
					{
					*ptr = ntohs(*ptr);
					ptr ++;
					}
				msg->vec.length = length;
				ok = 1;
				}
			}
		}
	return ok;
	}

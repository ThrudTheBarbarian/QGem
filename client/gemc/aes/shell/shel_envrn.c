//
//  shel_envrn.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/19/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "map.h"

/*****************************************************************************\
|* Provide some permanent storage for env vars to be referenced from
\*****************************************************************************/
static map_str_t 	_env;
static int			_mapOk = 0;

/*****************************************************************************\
|*  7900 : shel_envrn()
|*		 : obtains the value of an environment variable
\*****************************************************************************/
int16_t shel_envrn(char **value, char *name)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			{
			WARN("Cannot connect to GEMd service");
			if (value != NULL)
				*value = NULL;
			return 1;
			}
	
	/*************************************************************************\
	|* Check to see if the variable is valid
	\*************************************************************************/
	if (name == NULL)
		{
		WARN("Passed NULL variable to shel_envrn");
		if (value != NULL)
			*value = NULL;
		return 1;
		}
	
	/*************************************************************************\
	|* For historical reasons, the key passed in may have an '=' sign appended
	|* to it. If so, remove it.
	\*************************************************************************/
	char *saneName 	= name;
	uint32_t len 	= (uint32_t) strlen(name);
	if (name[len-1] == '=')
		{
		saneName = strdup(name);
		saneName[len-1] = '\0';
		len --;
		}
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_AES_SHEL_ENVRN);
	_gemMsgAppendData(&msg, (uint8_t *)saneName, len);
	_gemIoWrite(&msg);

	/*************************************************************************\
	|* Initialise the map if necessary
	\*************************************************************************/
	if (_mapOk == 0)
		{
		map_init(&_env);
		_mapOk = 1;
		}
		
	/*************************************************************************\
	|* Wait for a response
	\*************************************************************************/
	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_AES_SHEL_ENVRN));

	/*************************************************************************\
	|* Retrieve the string value, if it's length is 0, we return "", and if
	|* it's not found, we return NULL
	\*************************************************************************/
	int16_t found = ntohs(msg.vec.data[0]);
	if (found != 0)
		{
		uint32_t length = _gemMsgDataLength(&msg, 1);
		if (length == 0)
			{
			if (value)
				*value[0] = '\0';
			}
		else
			{
			char result[length+1];
			_gemMsgFetchData(&msg, 3, (uint8_t *)result, length);
			
			char **existing = map_get(&_env, saneName);
			if (existing != NULL)
				map_remove(&_env, saneName);
			
			map_set(&_env, saneName, strdup(result));
			if (value)
				{
				char **val = map_get(&_env, saneName);
				*value= *val;
				}
			}
		}
	else
		{
		if (value)
			*value = NULL;
		}
	
	/*************************************************************************\
	|* Un-modify if necessary before returning
	\*************************************************************************/
	if (saneName != name)
		free(saneName);
		
	return 1;
	}


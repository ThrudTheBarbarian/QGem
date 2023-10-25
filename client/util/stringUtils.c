//
//  stringUtils.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/25/23.
//

#include <ctype.h>
#include <string.h>

#include "stringUtils.h"

/*****************************************************************************\
|* Trims whitespace from a string. Requires write-access to the string and
|* returns the new pointer. Note that this will cause an error if free() is
|* called on the returned pointer (since it won't match what was malloc'd)
\*****************************************************************************/
char * strTrim(char *str)
	{
	char *end;

	// Trim leading space
	while(isspace((unsigned char)*str))
		str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end))
		end--;

	// Write new null terminator character
	end[1] = '\0';

	return str;
	}

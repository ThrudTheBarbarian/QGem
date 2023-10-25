//
//  stringUtils.h
//  gemc
//
//  Created by ThrudTheBarbarian on 10/25/23.
//

#ifndef stringUtils_h
#define stringUtils_h

#include <stdio.h>

/*****************************************************************************\
|* Trims whitespace from a string. Requires write-access to the string and
|* returns the new pointer. Note that this will cause an error if free() is
|* called on the returned pointer (since it won't match what was malloc'd)
\*****************************************************************************/
char * strTrim(char *str);

#endif /* stringUtils_h */

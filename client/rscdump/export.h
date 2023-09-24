//
//  export.h
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/24/23.
//

#ifndef export_h
#define export_h

#include <stdio.h>

struct Object_t;

/*****************************************************************************\
|* Export an object to a file
\*****************************************************************************/
int exportObject(struct Object_t *o);

/*****************************************************************************\
|* Export an object to a named file. The name is used as a stem for any outputs
\*****************************************************************************/
int exportObjectTo(struct Object_t *o, const char *name);

#endif /* export_h */

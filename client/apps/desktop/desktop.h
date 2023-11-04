//
//  desktop.h
//  client
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#ifndef desktop_h
#define desktop_h

#include "rscfile.h"
#include "shell_conv.h"
#include "vec.h"

/*****************************************************************************\
|* Types
\*****************************************************************************/
typedef struct
	{
	int16_t handle;						// As told us by the server
	int16_t xywh[4];					// I shouldn't have to comment these
	int16_t isRoot;						// Is this the root window ? (1|0)
	int16_t bgColour[3];				// RGB values of background
	int16_t bgPattern;					// GEM pattern to draw in background
	int		controls;					// Bitmap of controls
	} Window;
	

typedef vec_t(Window)	vec_window_t;	// A vector of windows...

typedef struct
	{
	int16_t handle;						// Connection handle
	ND_INFO env;						// Desktop environment, parsed
	vec_window_t wins;					// List of windows, including root
	} DesktopContext;


#endif /* desktop_h */

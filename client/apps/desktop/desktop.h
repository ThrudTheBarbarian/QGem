//
//  desktop.h
//  client
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#ifndef desktop_h
#define desktop_h

#include <limits.h>

#include "rscfile.h"
#include "shell_conv.h"
#include "vec.h"

/*****************************************************************************\
|* Types
\*****************************************************************************/

typedef struct
	{
	int16_t x __attribute__ ((aligned (2)));
	int16_t y __attribute__ ((aligned (2)));
	int16_t w __attribute__ ((aligned (2)));
	int16_t h __attribute__ ((aligned (2)));
	} Rect;

typedef struct
	{
	int16_t 	handle;					// As told us by the server
	Rect 		at;						// Window size and location
	int16_t 	isRoot;					// Is this the root window ? (1|0)
	int16_t 	bgColour[3];			// RGB values of background
	int16_t 	bgPattern;				// GEM pattern to draw in background
	int			controls;				// Bitmap of controls
	int 		visible;				// Visible (1) or not (0)
	vec_int_t	icons;					// List of icons in the window
	int16_t		vs;						// Vertical slider position
	int16_t		hs;						// Horizontal slider position;
	char		path[PATH_MAX];			// What is being shown in the window
	} Window;
	
typedef struct
	{
	int identifier;						// Icon identifier
	Rect at;							// Location and size
	int selected;						// Whether selected
	MFDB icon;							// Ready-to-draw icon image
	MFDB mask;							// Ready-to-draw icon mask
	char text[64];						// Icon text underneath
	} Icon;

#define DESKICON_DRIVE(x)	(1000 + x)
#define DESKICON_TRASH(x)	(2000 + x)
#define DESKICON_PRINTER(x)	(3000 + x)
#define DESKICON_FILE(x)	(4000 + x)
#define DESKICON_FOLDER(x)	(6000 + x)


typedef vec_t(Window)	vec_window_t;	// A vector of windows...
typedef vec_t(Icon)		vec_icon_t;		// A vector of icons...

typedef struct
	{
	int16_t handle;						// Connection handle
	ND_INFO env;						// Desktop environment, parsed
	vec_window_t wins;					// List of windows, including root
	vec_icon_t icons;					// List of icons, including root
	} DesktopContext;


#endif /* desktop_h */

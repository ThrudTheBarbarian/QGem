//
//  draw.h
//  desktop
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#ifndef draw_h
#define draw_h

#include <limits.h>

#include "aes.h"
#include "debug.h"
#include "desktop.h"
#include "rscfile.h"

#include <stdio.h>

/*****************************************************************************\
|* Render the desktop
\*****************************************************************************/
void render(DesktopContext *ctx, Rect dirty);

#endif /* draw_h */

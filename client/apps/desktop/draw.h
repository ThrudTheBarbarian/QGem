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
#include "rscfile.h"

#include <stdio.h>

/*****************************************************************************\
|* Render the desktop
\*****************************************************************************/
void render(DesktopContext *ctx);

/*****************************************************************************\
|* Convert an indexed icon into MFDB's (mask and pixels) ready to draw
\*****************************************************************************/
int iconToMFDB(int idx, MFDB *mfdb, MFDB *mask);

#endif /* draw_h */

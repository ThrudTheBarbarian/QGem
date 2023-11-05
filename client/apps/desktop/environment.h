//
//  environment.h
//  desktop
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#ifndef environment_h
#define environment_h

#include "desktop.h"

/*****************************************************************************\
|* Read the environment from the server, and parse into more useful form
\*****************************************************************************/
void envRead(DesktopContext *ctx);

/*****************************************************************************\
|* Convert an indexed icon into MFDB's (mask and pixels) ready to draw
\*****************************************************************************/
int iconToMFDB(int idx, MFDB *mfdb, MFDB *mask);

#endif /* environment_h */

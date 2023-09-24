//
//  pam.h
//  rscdump
//
//  Created by Simon Gornall on 9/24/23.
//

#ifndef pam_h
#define pam_h

#include <stdio.h>

#include "gem.h"

/*****************************************************************************\
|* Write a PAM file from image data, with an optional mask
\*****************************************************************************/
int writePAM(const char *filename, MFDB *imgData, MFDB *imgMask);

#endif /* pam_h */

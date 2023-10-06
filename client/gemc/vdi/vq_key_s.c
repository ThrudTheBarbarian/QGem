//
//  vq_key_s.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/5/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  128  : Sample keyboard state
\*****************************************************************************/
void vq_key_s(int16_t handle, int16_t *status)
	{
	_gemIoMouseState(NULL, NULL, NULL, status);
	}

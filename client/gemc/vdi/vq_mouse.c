//
//  vq_mouse.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/5/23.
//

#include <stdio.h>

#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  124  : Get the current mouse state
\*****************************************************************************/
void vq_mouse(int16_t handle, int16_t *status, int16_t *x, int16_t *y)
	{
	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	_gemIoMouseState(x, y, status, NULL);
	}

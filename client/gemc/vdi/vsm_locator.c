//
//  vsm_locator.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/17/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "vdi.h"

/*****************************************************************************\
|*   28 : Sample the mouse position from the last-received event
\*****************************************************************************/
void vsm_locator(int16_t handle, int16_t x, int16_t y,
				 int16_t* x1, int16_t* y1, int16_t *term)
	{
	(void)handle;
	(void)x;
	(void)y;
	
	/*************************************************************************\
	|* Get the last-received mouse state
	\*************************************************************************/
	int16_t mx, my, mb;
	_gemIoMouseState(&mx, &my, &mb, NULL);

	/*************************************************************************\
	|* Copy data over if space is allocated
	\*************************************************************************/
	if (x1 != NULL)
		*x1 = mx;
	if (y1 != NULL)
		*y1 = my;
	if (term != NULL)
		{
		*term = (mb & 0x1) ? 32
			  : (mb & 0x2) ? 33
			  : 0;
		}
	}

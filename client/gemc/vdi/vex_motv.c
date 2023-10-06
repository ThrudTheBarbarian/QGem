//
//  vex_motv.c
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
|*  Static variables
\*****************************************************************************/
static GemMouseMoveFunction		_currentFn = NULL;

/*****************************************************************************\
|*  125  : Register a mouse-button-pressed function
\*****************************************************************************/
void vex_motv(int16_t handle,
			  GemMouseMoveFunction newFunc,
			  GemMouseMoveFunction *oldFunc)
	{
	if (oldFunc)
		*oldFunc	= _currentFn;
	_currentFn 	= newFunc;
	}

/*****************************************************************************\
|* Called by the message-receiver code while the timer vector is enabled
\*****************************************************************************/
void _gemProcessMoveEvent(int x, int y)
	{
	if (_currentFn != NULL)
		_currentFn(x, y);
	}

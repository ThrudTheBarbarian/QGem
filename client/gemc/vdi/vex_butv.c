//
//  vex_butv.c
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
static GemMouseButtonFunction		_currentFn = NULL;

/*****************************************************************************\
|*  125  : Register a mouse-button-pressed function
\*****************************************************************************/
void vex_butv(int16_t handle,
			  GemMouseButtonFunction newFunc,
			  GemMouseButtonFunction *oldFunc)
	{
	if (oldFunc)
		*oldFunc	= _currentFn;
	_currentFn 	= newFunc;
	}

/*****************************************************************************\
|* Called by the message-receiver code while the timer vector is enabled
\*****************************************************************************/
void _gemProcessButtonEvent(int mouseState)
	{
	if (_currentFn != NULL)
		_currentFn(mouseState);
	}

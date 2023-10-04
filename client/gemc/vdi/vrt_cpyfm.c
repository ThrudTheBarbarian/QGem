//
//  vrt_cpyfm.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/4/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  121  : Blit a monochrome raster onto a colour raster
\*****************************************************************************/
void vrt_cpyfm (int16_t handle,
				int16_t mode,
				int16_t *pxy8,
				MFDB *src,
				MFDB *dst,
				int16_t *colourIndex)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Make sure the values passed in are all ok
	\*************************************************************************/
	if ((dst == NULL) || (src == NULL) || (colourIndex == NULL))
		{
		WARN("Parameter error in vrt_cpyfm");
		return;
		}
		
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VRT_CPYFM);
	_gemMsgAppend(&msg, &mode, 1);
	_gemMsgAppend(&msg, colourIndex, 2);
	_gemMsgAppend(&msg, pxy8, 8);
	_gemMsgAppendMfdb(&msg, src);
	_gemMsgAppendMfdb(&msg, dst);
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}


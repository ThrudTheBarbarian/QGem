//
//  vro_cpyfm.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/19/23.
//

#include <stdio.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"

/*****************************************************************************\
|*  Forward refs
\*****************************************************************************/
void append(GemMsg *msg, MFDB *mfdb);

/*****************************************************************************\
|*  109  : Blit an opaque rect
\*****************************************************************************/
void vro_cpyfm(int16_t handle, int16_t mode, int16_t *pxy,
			   MFDB *src, MFDB *dst)
	{
	/*************************************************************************\
	|* Check to see if we're connected
	\*************************************************************************/
	if (!_gemIoIsConnected())
		if (!_gemIoConnect())
			return;
	
	/*************************************************************************\
	|* Construct and send the message
	\*************************************************************************/
	GemMsg msg;
	_gemMsgInit(&msg, MSG_VRO_CPYFM);
	_gemMsgAppend(&msg, &mode, 1);
	_gemMsgAppend(&msg, pxy, 8);
	append(&msg, src);
	append(&msg, dst);
	
	_gemIoWrite(&msg);
			
	/*************************************************************************\
	|* Clear the message allocations
	\*************************************************************************/
	_gemMsgDestroy(&msg);
	}

/*****************************************************************************\
|* Append an MFDB to a message
\*****************************************************************************/
void append(GemMsg *msg, MFDB *mfdb)
	{
	int16_t needsData = (mfdb->fd_addr == 0) ? 0 : 1;
	
	_gemMsgAppend(msg, &needsData, 1);
	_gemMsgAppend(msg, &(mfdb->fd_w), 1);
	_gemMsgAppend(msg, &(mfdb->fd_h), 1);
	_gemMsgAppend(msg, &(mfdb->fd_wdwidth), 1);
	_gemMsgAppend(msg, &(mfdb->fd_stand), 1);
	_gemMsgAppend(msg, &(mfdb->fd_nplanes), 1);
	_gemMsgAppend(msg, &(mfdb->fd_r1), 1);
	_gemMsgAppend(msg, &(mfdb->fd_r2), 1);
	_gemMsgAppend(msg, &(mfdb->fd_r3), 1);
	
	if (needsData)
 		_gemMsgAppendData(msg,
 						  mfdb->fd_addr,
 						  mfdb->fd_wdwidth * 2 * mfdb->fd_h);
	}

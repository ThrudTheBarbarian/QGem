//
//  rsrc_load.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/11/23.
//

#include <stdio.h>

#include "aes.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "rscfile.h"

static RscFile *_rsc = NULL;


/*****************************************************************************\
|*  7802 : rsrc_load()
|*		 : Load up the RSC file for this application
\*****************************************************************************/
int16_t rsrc_load(const char *filename)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* Check to see if we need to free any old one
	\*************************************************************************/
	if (_rsc != NULL)
		{
		rsrc_free();
		_rsc = NULL;
		}
		
	/*************************************************************************\
	|* Create an RSC pointer
	\*************************************************************************/
	_rsc = (RscFile *) calloc(sizeof(RscFile), 1);
	if (_rsc != NULL)
		{
		/*********************************************************************\
		|* Load it up
		\*********************************************************************/
		ok = resourceLoad(filename, _rsc);
		}
		
	return ok;
	}


/*****************************************************************************\
|*  Return the RscFile structure for this application
\*****************************************************************************/
RscFile * _rsrcGet(void)
	{
	return _rsc;
	}

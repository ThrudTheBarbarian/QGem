//
//  environment.c
//  desktop
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#include "aes.h"
#include "debug.h"
#include "environment.h"
#include "gem.h"

#undef DBG_ICONS

/*****************************************************************************\
|* Read the environment from the display service
\*****************************************************************************/
void envRead(DesktopContext *ctx)
	{
	/*************************************************************************\
	|* Load the resource file containing icons
	\*************************************************************************/
	char path[PATH_MAX];
	#ifdef DBG_ICONS
		rsrc_load("/usr/local/atari/Disk/Applications/desktop.app/icons.rsc");
	#else
		strcpy(path, "icons.rsc");
		if (!shel_find(path))
			WARN("Cannot find resource file for icons!");
		else
			{
			if (rsrc_load(path) == 0)
				WARN("Failed to load resource file for icons (%s)", path);
			}
	#endif
	
	/*************************************************************************\
	|* Read the desktop environment from the server
	\*************************************************************************/
	char envBuffer[63*1024];
	if (!shel_get(envBuffer, 63*1024))
		WARN("Cannot get the desktop environment!");
	else
		{
		_gemInfZero(&(ctx->env));
		_gemInfInit(&(ctx->env));
		_gemInfReadData(envBuffer, &(ctx->env));
		}
	}

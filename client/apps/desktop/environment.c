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
|* Forward declarations
\*****************************************************************************/
static void _addDriveIcon(DesktopContext *ctx, ND_DRIVE* drv, int identifier);



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
	
	
	/*************************************************************************\
	|* Create icons for any references we find
	\*************************************************************************/
	for (int i=0; i<ctx->env.drives.length; i++)
		_addDriveIcon(ctx, ctx->env.drives.data[i], DESKICON_DRIVE(i));

	if (ctx->env.printer.iconId != 0)
		_addDriveIcon(ctx, &(ctx->env.printer), DESKICON_PRINTER(0));

	if (ctx->env.trash.iconId != 0)
		_addDriveIcon(ctx, &(ctx->env.trash), DESKICON_TRASH(0));

	}

/*****************************************************************************\
|* Private function: add a drive icon to the icons list
\*****************************************************************************/
static void _addDriveIcon(DesktopContext *ctx, ND_DRIVE* drv, int identifier)
	{
	Icon icon;
	icon.identifier	= identifier;
	int x			= 16 + drv->x * 63;
	int y			= drv->y * 66;
	icon.at			= (Rect){x, y, 48, 48};
	icon.selected	= 0;
	iconToMFDB(drv->iconId, &(icon.icon), &(icon.mask));
	strncpy(icon.text, drv->text, 32);
	
	vec_push(&(ctx->icons), icon);
	}
	
/*****************************************************************************\
|* Convert an icon index into an MFDB
\*****************************************************************************/
int iconToMFDB(int idx, MFDB *icon, MFDB *mask)
	{
	int ok = 0;
	RscFile *rsc = _rsrcGet();
	if (idx >=0 && idx < rsc->nCicons)
		{
		if (icon != NULL)
			{
			icon->fd_addr 		= rsc->cIcons[idx].icons[1].colData;
			icon->fd_w			= rsc->cIcons[idx].monoIcon.ib_wicon;
			icon->fd_h			= rsc->cIcons[idx].monoIcon.ib_hicon;
			icon->fd_stand		= MFDB_STANDARD;
			icon->fd_nplanes	= rsc->cIcons[idx].icons[1].numPlanes;
			icon->fd_r1			= 0;
			icon->fd_r2			= 0;
			icon->fd_r3			= 0;
			icon->fd_wdwidth	= ((icon->fd_w)/16) + ((icon->fd_w & 15) != 0
								? 1
								: 0);
			}
		if (mask != NULL)
			{
			mask->fd_addr 		= rsc->cIcons[idx].monoIcon.ib_pmask;
			mask->fd_w			= rsc->cIcons[idx].monoIcon.ib_wicon;
			mask->fd_h			= rsc->cIcons[idx].monoIcon.ib_hicon;
			mask->fd_stand		= MFDB_STANDARD;
			mask->fd_nplanes	= 1;
			mask->fd_r1			= 0;
			mask->fd_r2			= 0;
			mask->fd_r3			= 0;
			mask->fd_wdwidth	= ((mask->fd_w)/16) + ((mask->fd_w & 15) != 0
								? 1
								: 0);
			}
		ok = 1;
		}
	return ok;
	}

//
//  draw.c
//  desktop
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#include <limits.h>

#include "desktop.h"
#include "draw.h"


/*****************************************************************************\
|* Draw an icon
\*****************************************************************************/
static void _drawDriveIcon(DesktopContext *ctx, ND_DRIVE *drv)
	{
	int x = 16 + drv->x * 63;
	int y = drv->y * 66;
	
	MFDB icon, mask;
	if (iconToMFDB(drv->iconId, &icon, &mask))
		{
		MFDB dst;
		memset(&dst, 0, sizeof(MFDB));
		MFDB screen = dst;

		/*********************************************************************\
		|* Decide where to draw
		\*********************************************************************/
		int w = icon.fd_w;
		int h = icon.fd_h;
		
		int16_t pxy[8]	=
			{
			0, 0, w-1,   h-1,
			x, y, x+w-1, y+h-1,
			};
		
		/*********************************************************************\
		|* Cut out the hole in the screen pixmap
		\*********************************************************************/
		int16_t cols[] = {255,0};
		vr_trnfm(ctx->handle, &mask, &dst);
		vrt_cpyfm(ctx->handle, WR_TRANSPARENT, pxy, &dst, &screen, cols);
		
		dst = screen;
		vr_trnfm(ctx->handle, &icon, &dst);
		int16_t mode = ((int16_t)0x8000) | S_OR_D;
		vro_cpyfm(ctx->handle, mode, pxy, &dst, &screen);
		v_justified(ctx->handle, x-16, y+h, drv->text, w+32, 0, 0);
		}
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

	
/*****************************************************************************\
|* Draw the screen
\*****************************************************************************/
void render(DesktopContext *ctx)
	{
	vsf_style(ctx->handle, FIS_SOLID);
	vs_color(ctx->handle, 3, ctx->wins.data[0].bgColour);
	vsf_color(ctx->handle, 3);
	vsf_perimeter(ctx->handle, 0);
	v_bar(ctx->handle, ctx->wins.data[0].xywh);
	vst_color(ctx->handle, 255);
	vst_alignment(ctx->handle, TA_CENTER, TA_BASE, NULL, NULL);
	
	for (int i=0; i<ctx->env.drives.length; i++)
		{
		ND_DRIVE *drv = ctx->env.drives.data[i];
		_drawDriveIcon(ctx, drv);
		}
	if (ctx->env.printer.iconId != 0)
		_drawDriveIcon(ctx, &(ctx->env.printer));
	if (ctx->env.trash.iconId != 0)
		_drawDriveIcon(ctx, &(ctx->env.trash));
	}

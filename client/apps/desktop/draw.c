//
//  draw.c
//  desktop
//
//  Created by ThrudTheBarbarian on 11/3/23.
//

#include <limits.h>

#include "draw.h"
#include "geometry.h"

/*****************************************************************************\
|* Draw an icon
\*****************************************************************************/
static void _drawIcon(DesktopContext *ctx, int iconId, Rect r)
	{
	for (int i=0; i<ctx->icons.length; i++)
		{
		Icon *icon = &(ctx->icons.data[i]);
		if (icon->identifier == iconId)
			{
			// Early exit if no overlap
			if (intersection(r, icon->at, NULL) == 0)
				return;
			
			vs_clip(ctx->handle, 1, (int16_t *)(&r));
			
			MFDB dst;
			memset(&dst, 0, sizeof(MFDB));
			MFDB screen = dst;

			/*****************************************************************\
			|* Decide where to draw
			\*****************************************************************/
			int x = icon->at.x;
			int y = icon->at.y;
			int w = icon->at.w;
			int h = icon->at.h;
			
			int16_t pxy[8]	=
				{
				0, 0, w-1,   h-1,
				x, y, x+w-1, y+h-1,
				};
			
			/*****************************************************************\
			|* Cut out the hole in the screen pixmap
			\*****************************************************************/
			int16_t cols[] = {255,0};
			vr_trnfm(ctx->handle, &(icon->mask), &dst);
			vrt_cpyfm(ctx->handle, WR_TRANSPARENT, pxy, &dst, &screen, cols);
			
			dst = screen;
			vr_trnfm(ctx->handle, &(icon->icon), &dst);
			int16_t mode = ((int16_t)0x8000) | S_OR_D;
			vro_cpyfm(ctx->handle, mode, pxy, &dst, &screen);
			v_justified(ctx->handle, x-16, y+h, icon->text, w+32, 0, 0);

			vs_clip(ctx->handle, 0, (int16_t *)(&r));
			}
		}
	}


	
/*****************************************************************************\
|* Draw the background
\*****************************************************************************/
static void _renderBackground(DesktopContext *ctx, Rect dirty)
	{
	int16_t x,y,w,h;

	wind_get(ctx->wins.data[0].handle, WF_FIRSTXYWH, &x, &y, &w, &h);
	while ((w>0) || (h>0))
		{
		Rect r;
		Rect ours = (Rect){x,y,w,h};
		if (intersection(dirty, ours, &r))
			{
			/****************************************************************\
			|* First draw the background
			\*****************************************************************/
			printf("bg: x=%d, y=%d, w=%d, h=%d\n", r.x, r.y, r.w, r.h);
			
			vsf_style(ctx->handle, FIS_SOLID);
			vs_color(ctx->handle, 3, ctx->wins.data[0].bgColour);
			vsf_color(ctx->handle, 3);
			vsf_perimeter(ctx->handle, 0);
			v_bar(ctx->handle, (int16_t *)&r);

			/****************************************************************\
			|* If this rect intersects any of the icons, draw then
			\*****************************************************************/
			vst_color(ctx->handle, 255);
			vst_alignment(ctx->handle, TA_CENTER, TA_BASE, NULL, NULL);

			for (int i=0; i<ctx->env.drives.length; i++)
				_drawIcon(ctx, DESKICON_DRIVE(i), r);
			if (ctx->env.printer.iconId != 0)
				_drawIcon(ctx, DESKICON_PRINTER(0), r);
			if (ctx->env.trash.iconId != 0)
				_drawIcon(ctx, DESKICON_TRASH(0), r);

			}
			
		wind_get(ctx->wins.data[0].handle, WF_NEXTXYWH, &x, &y, &w, &h);
		}
	}

/*****************************************************************************\
|* Draw a window
\*****************************************************************************/
static void _renderWindow(DesktopContext *ctx, Rect dirty, Window *win)
	{
	int16_t x,y,w,h;

	wind_get(win->handle, WF_FIRSTXYWH, &x, &y, &w, &h);
	while ((w>0) || (h>0))
		{
		Rect r;
		Rect ours = (Rect){x,y,w,h};
		if (intersection(dirty, ours, &r))
			{
			/****************************************************************\
			|* First draw the background
			\*****************************************************************/
			printf("win %d: x=%d, y=%d, w=%d, h=%d\n", win->handle, r.x, r.y, r.w, r.h);
			
			vsf_style(ctx->handle, win->bgPattern);
			vs_color(ctx->handle, 254, win->bgColour);
			vsf_color(ctx->handle, 254);
			vsf_perimeter(ctx->handle, 0);
			v_bar(ctx->handle, (int16_t *)&r);
			}
		wind_get(win->handle, WF_NEXTXYWH, &x, &y, &w, &h);
		}
	}
	
/*****************************************************************************\
|* Draw the screen
\*****************************************************************************/
void render(DesktopContext *ctx, Rect dirty)
	{
	_renderBackground(ctx, dirty);
	
	for (int i=1; i<ctx->wins.length; i++)
		{
		Window *w = &(ctx->wins.data[i]);
		if (w->visible)
			{
			_renderWindow(ctx, dirty, w);
			}
		}
	}


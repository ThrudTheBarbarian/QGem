/*****************************************************************************\
|*
|*  main.c
|*  desktop
|*
|*  Created by ThrudTheBarbarian on 10/23/23.
|*
|* Implements a desktop-like interface for QGem
\*****************************************************************************/

#undef DBG_ENVIRON
#define DBG_ICONS

#include <limits.h>

#include "aes.h"
#include "debug.h"
#include "rscfile.h"
#include "shell_conv.h"

/*****************************************************************************\
|* Types
\*****************************************************************************/
typedef struct
	{
	int16_t handle;					// Connection handle
	int16_t bg[3];					// Colour of the background
	int rootWindow;					// Handle of the root window
	int16_t xywh[4];				// Dimensions of the root window
	ND_INFO env;					// Desktop environment, parsed
	} DesktopContext;
	
/*****************************************************************************\
|* Forward references
\*****************************************************************************/
void render(DesktopContext *ctx);
int iconToMFDB(int idx, MFDB *mfdb, MFDB *mask);

/*****************************************************************************\
|* Program entry
\*****************************************************************************/
int main(int argc, const char * argv[], const char *env[])
	{
	int i;				/* The overworked minority of integer loop variables */
	int16_t workIn[16];
	int16_t msgBuf[8];
	int16_t workOut[128];
	
	for (i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;

	/*************************************************************************\
	|* Set up the DesktopContext
	\*************************************************************************/
	DesktopContext ctx;
	ctx.bg[0] = 161000 / 256;
	ctx.bg[1] = 217000 / 256;
	ctx.bg[2] = 137000 / 256;
	
	/*************************************************************************\
	|* Initialise the application and set up the connection to the server
	\*************************************************************************/
	int appHandle = appl_init();

	char cmd[PATH_MAX];
	char args[PATH_MAX];
	shel_read(cmd, args);
	printf("shel_read: [%s] [%s]\n", cmd, args);

	#ifdef DBG_ENVIRON
	/*************************************************************************\
	|* print out the environment
	\*************************************************************************/
	int idx = 0;
	while (env[idx])
		printf("%s\n", env[idx++]);
	#endif
	
	/*************************************************************************\
	|* Connect to the display, clear it, and ensure we're in graphics mode
	\*************************************************************************/
	v_opnvwk(workIn, &(ctx.handle), workOut);
	v_clrwk(ctx.handle);
	v_exit_cur(ctx.handle);
	
	/*************************************************************************\
	|* Pull some "important" values out of the connection data
	\*************************************************************************/
	int xMax 	= workOut[0];
	int yMax	= workOut[1];
	
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
		_gemInfZero(&(ctx.env));
		_gemInfInit(&(ctx.env));
		_gemInfReadData(envBuffer, &(ctx.env));
		}
	
	/*************************************************************************\
	|* Create and open a window - it's going to be full-screened no matter what
	|* size we set, since we're the desktop
	\*************************************************************************/
	ctx.rootWindow = wind_create(-1, 0, 0, xMax, yMax);
	wind_open(ctx.rootWindow, 0, 0, xMax, yMax);

	/*************************************************************************\
	|* Wait for the notification that the window opened
	\*************************************************************************/
	evnt_mesag(msgBuf);
	memcpy(ctx.xywh, &(msgBuf[4]), 4*sizeof(int16_t));
	
	/*************************************************************************\
	|* Draw something
	\*************************************************************************/
	render(&ctx);
	}

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }

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
|* Draw the screen
\*****************************************************************************/
void render(DesktopContext *ctx)
	{
	vsf_style(ctx->handle, FIS_SOLID);
	vs_color(ctx->handle, 3, ctx->bg);
	vsf_color(ctx->handle, 3);
	vsf_perimeter(ctx->handle, 0);
	v_bar(ctx->handle, ctx->xywh);
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

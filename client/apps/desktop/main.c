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
#undef DBG_ICONS

#include "desktop.h"
#include "draw.h"
#include "environment.h"

/*****************************************************************************\
|* Local defines
\*****************************************************************************/
#define WINDOW_CONTROLS		0xFFFF

/*****************************************************************************\
|* Forward references
\*****************************************************************************/

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
	vec_init(&(ctx.wins));
	vec_init(&(ctx.icons));
	
	
	/*************************************************************************\
	|* Add the root window in as 'window 0'
	\*************************************************************************/
	Window root;
	root.bgColour[0] = 161000 / 256;
	root.bgColour[1] = 217000 / 256;
	root.bgColour[2] = 137000 / 256;
	vec_push(&(ctx.wins), root);
	
	/*************************************************************************\
	|* Initialise the application and set up the connection to the server
	\*************************************************************************/
	int appHandle = appl_init();

	char cmd[PATH_MAX];			// The actual command to run
	char args[PATH_MAX];		// The arguments to pass it
	shel_read(cmd, args);

	#ifdef DBG_ENVIRON
		/*********************************************************************\
		|* print out the environment
		\*********************************************************************/
		printf("shel_read: [%s] [%s]\n", cmd, args);
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
	int xMax = workOut[0];
	int yMax = workOut[1];
	ctx.wins.data[0].at.x = 0;
	ctx.wins.data[0].at.y = 0;
	ctx.wins.data[0].at.w = xMax+1;
	ctx.wins.data[0].at.h = yMax+1;
	
	/*************************************************************************\
	|* Load the resource file containing icons
	\*************************************************************************/
	envRead(&ctx);
	
	/*************************************************************************\
	|* Create and open a window - it's going to be full-screened no matter what
	|* size we set, since we're the desktop
	\*************************************************************************/
	ctx.wins.data[0].handle = wind_create(-1, 0, 0, xMax, yMax);
	wind_open(ctx.wins.data[0].handle, 0, 0, xMax, yMax);

	/*************************************************************************\
	|* Wait for the notification that the window opened
	\*************************************************************************/
	evnt_mesag(msgBuf);
	memcpy(&(ctx.wins.data[0].at), &(msgBuf[4]), 4*sizeof(int16_t));

	/*************************************************************************\
	|* Fetch any of the windows defined in the desktop environment, open or not
	\*************************************************************************/
	Rect all = (Rect) {0, 0, xMax+1, yMax+1};
	for (int i=0; i<ctx.env.windows.length; i++)
		{
		ND_WINDOW *eWin = ctx.env.windows.data[i];
		Window win;
		
		win.visible 	= (eWin->status == 0) ? 0 : 1;
		win.at.x		= eWin->x;
		win.at.y		= eWin->y;
		win.at.w		= eWin->w;
		win.at.h 		= eWin->h;
		win.vs			= eWin->vs;
		win.hs			= eWin->hs;
		win.bgColour[0]	= 1000;
		win.bgColour[1]	= 1000;
		win.bgColour[2] = 1000;
		win.bgPattern	= 0;
		win.controls	= -1;
		if (strlen(eWin->pathSpec) > 0)
			strncpy(win.path, eWin->pathSpec, PATH_MAX);
		vec_init(&(win.icons));

		if (win.visible)
			{
			win.handle	= wind_create(win.controls, all.x, all.y, all.w, all.h);
			if (win.handle > 0)
				wind_open(win.handle, win.at.x, win.at.y, win.at.w, win.at.h);
			}
		vec_push(&(ctx.wins), win);
		}

	/*************************************************************************\
	|* Draw the entire screen to start off with
	\*************************************************************************/
	render(&ctx, all);

	}

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }
	

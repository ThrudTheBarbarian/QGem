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

#include <limits.h>

#include "aes.h"

int main(int argc, const char * argv[], const char *env[])
	{
	int i;				/* The overworked minority of integer loop variables */
	int16_t workIn[16];
	int16_t msgBuf[8];
	int16_t workOut[128];
	int16_t handle;
	int16_t bg[3];
	
	for (i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;

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
	v_opnvwk(workIn, &handle, workOut);
	v_clrwk(handle);
	v_exit_cur(handle);
	
	/*************************************************************************\
	|* Pull some "important" values out of the connection data
	\*************************************************************************/
	int xMax 	= workOut[0];
	int yMax	= workOut[1];
	
	/*************************************************************************\
	|* Create and open a window - it's going to be full-screened no matter what
	|* size we set, since we're the desktop
	\*************************************************************************/
	int16_t rootWin = wind_create(-1, 0, 0, xMax, yMax);
	wind_open(rootWin, 0, 0, xMax, yMax);
	
	/*************************************************************************\
	|* Set up the colours
	\*************************************************************************/
	bg[0] = 161000 / 256;
	bg[1] = 217000 / 256;
	bg[2] = 137000 / 256;
	
	/*************************************************************************\
	|* Wait for the notification that the window opened
	\*************************************************************************/
	evnt_mesag(msgBuf);
	
	/*************************************************************************\
	|* Draw something
	\*************************************************************************/
	vsf_style(handle, FIS_SOLID);
	vs_color(handle, 3, bg);
	vsf_color(handle, 3);
	vsf_perimeter(handle, 0);
	v_bar(handle, &(msgBuf[4]));
	}

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }
	

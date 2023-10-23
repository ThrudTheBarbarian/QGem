/*****************************************************************************\
|*
|*  main.c
|*  desktop
|*
|*  Created by ThrudTheBarbarian on 10/23/23.
|*
|* Implements a desktop-like interface for QGem
\*****************************************************************************/

#include "aes.h"

int main(int argc, const char * argv[], const char *env[])
	{
	int i;				/* The overworked minority of integer loop variables */
	int16_t workIn[16];
	int16_t workOut[128];
	int16_t handle;
	
	for (i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;

#if 0
	/*************************************************************************\
	|* print out the environment
	\*************************************************************************/
	FILE *fp = fopen("/tmp/log", "w");
	int idx = 0;
	while (env[idx])
		{
		fprintf(fp, "%s\n", env[idx]);
		idx ++;
		}
	fclose(fp);
#endif

	/*************************************************************************\
	|* Initialise the application and set up the connection to the server
	\*************************************************************************/
	int appHandle = appl_init();
	
	/*************************************************************************\
	|* Connect to the display, clear it, and ensure we're in graphics mode
	\*************************************************************************/
	v_opnvwk(workIn, &handle, workOut);
	v_clrwk(handle);
	v_exit_cur(handle);
	}

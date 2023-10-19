/*****************************************************************************\
|*  main.c
|*  aspiral
|*
|*  Created by ThrudTheBarbarian on 10/17/23.
\*****************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
 
#include "gem.h"

#ifdef QGEM
#include "gemio.h"
#include "gemmsg.h"
#include "rscfile.h"
#include "vdi.h"
#endif

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
void plotArchimedesSpiral(int handle, int offX, int offY, int transparent);

/*****************************************************************************\
|* Archimedes spiral as a line-drawing demo
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	int i;				/* The overworked minority of integer loop variables */
	int16_t workIn[16];
	int16_t workOut[128];
	int16_t handle;
	
	for (i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;
	
	/*************************************************************************\
	|* Connect to the display, clear it, and ensure we're in graphics mode
	\*************************************************************************/
	v_opnvwk(workIn, &handle, workOut);
	v_clrwk(handle);
	v_exit_cur(handle);
	
	plotArchimedesSpiral(handle, 0,0, 1);
	plotArchimedesSpiral(handle, 320,0, 0);
	}

/*****************************************************************************\
|* Plot algorithm
\*****************************************************************************/
void plotArchimedesSpiral(int handle, int offX, int offY, int transparent)
	{
	
	/*************************************************************************\
	|* Set up the parameters
	\*************************************************************************/
	const  int kSize 	= 144;				/* Scaling horizontally 	*/
	int xp				= kSize;
	float xr			= 4.71238905f;
	float xf			= xr / xp;
	int xi, zi;								/* loop variables		 	*/
	float zt, zs, xl, xt, yy, x1, y1;		/* intermediate co-ords 	*/
	int16_t pxy[4];							/* co-ords to plot/draw		*/
	char buf[128];							/* temporary string buffer	*/
	struct timeval stt, end, dt;			/* used for timing			*/

	/*************************************************************************\
	|* Get the starting time
	\*************************************************************************/
	gettimeofday(&stt, NULL);

	/*************************************************************************\
	|* Draw the plot. Uses a painters algorithm, so we can overdraw in white if
	|* 'transparent' is set to zero
	\*************************************************************************/
	for (zi = -64; zi < 64; zi++)
		{
		zt = zi * 2.25f;
		zs = zt * zt;
		xl = (int)(sqrtf(kSize * kSize - zs) + 0.5f);
		
		for (xi = -xl; xi<xl; xi++)
			{
			xt = sqrt(xi*xi + zs) * xf;
			yy = (sin(xt) + sin(xt*3) * 0.4) * 56;
			x1 = offX + xi + zi + 160;
			y1 = offY + 90 - yy + zi;
			
			vsl_color(handle, 1);
			pxy[0] = pxy[2]	= (int16_t) x1;
			pxy[1] = pxy[3]	= (int16_t) y1;
			v_pline(handle, 2, pxy);
			
			if (transparent == 0)
				{
				pxy[0] = (int16_t)x1;
				pxy[1] = (int16_t)(y1+1);
				pxy[2] = (int16_t)x1;
				pxy[3] = 191;

				vsl_color(handle, 0);
				v_pline(handle, 2, pxy);
				}
			}
		}
	
	/*************************************************************************\
	|* Show how long it took
	\*************************************************************************/
	gettimeofday(&end, NULL);
	timersub(&end, &stt, &dt);
	sprintf(buf, "Time taken: %d.%06d secs", (int)dt.tv_sec, (int)dt.tv_usec);
	
	vst_alignment(handle, TA_CENTER, TA_BASE, NULL, NULL);
	v_justified(handle, offX, offY + 220, buf, 320, 0,0);
	}
	

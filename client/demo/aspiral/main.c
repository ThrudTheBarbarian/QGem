//
//  main.c
//  aspiral
//
//  Created by Simon Gornall on 10/17/23.
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
 
#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "rscfile.h"
#include "vdi.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
void plotArchimedesSpiral(int handle, int offX, int offY, int transparent);

/*****************************************************************************\
|* Archimedes spiral as a line-drawing demo
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	int16_t workIn[16];
	int16_t workOut[128];
	int16_t handle;
	
	for (int i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;
	
	/*************************************************************************\
	|* Connect to the display, clear it, and ensure we're in graphics mode
	\*************************************************************************/
	v_opnvwk(workIn, &handle, workOut);
	v_clrwk();
	vq_exit_cur(handle);
	
	plotArchimedesSpiral(handle, 0,0, 1);
	plotArchimedesSpiral(handle, 320,0, 0);
	}

/*****************************************************************************\
|* Plot algorithm
\*****************************************************************************/
void plotArchimedesSpiral(int handle, int offX, int offY, int transparent)
	{
	struct timeval stt, end, dt;
	
	gettimeofday(&stt, NULL);
	/*************************************************************************\
	|* Set up the parameters
	\*************************************************************************/
	const  int kSize 	= 144;
	int xp				= kSize;
	float xr			= 4.71238905f;
	float xf			= xr / xp;
	
	int16_t black[3] = {0,0,0};
	vsl_color(handle, 0);
	
	/*************************************************************************\
	|* Draw the plot
	\*************************************************************************/
	for (int zi = -64; zi < 64; zi++)
		{
		float zt = zi * 2.25f;
		float zs = zt * zt;
		float xl = (int)(sqrtf(kSize * kSize - zs) + 0.5f);
		
		for (int xi = -xl; xi<xl; xi++)
			{
			float xt = sqrt(xi*xi + zs) * xf;
			float yy = (sin(xt) + sin(xt*3) * 0.4) * 56;
			float x1 = offX + xi + zi + 160;
			float y1 = offY + 90 - yy + zi;
			
			vs_pixrgb(handle, x1, y1, black);
			
			if (transparent == 0)
				{
				int16_t pxy[4] = {(int16_t)x1, (int16_t)(y1+1),
								  (int16_t)x1, 191};
				v_pline(handle, 2, pxy);
				}
			}
		}
	
	/*************************************************************************\
	|* Show how long it took
	\*************************************************************************/
	gettimeofday(&end, NULL);
	timersub(&end, &stt, &dt);
	char buf[128];
	sprintf(buf, "Time taken: %d.%06d secs", (int)dt.tv_sec, (int)dt.tv_usec);
	
	vst_alignment(handle, ALGN_CENTER, ALGN_BASELINE, NULL, NULL);
	v_justified(handle, offX, offY + 220, 320, 0,0, buf);
	}
	

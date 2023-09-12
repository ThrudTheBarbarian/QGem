//
//  main.c
//  gemtest
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include "vdi.h"
#include "gem.h"

int debugLevel(void)
	{ return 10; }
	

int main(int argc, const char * argv[])
	{
	int16_t workIn[16];
	int16_t workOut[128];
	int16_t handle;
	
	for (int i=0; i<16; i++)
		workIn[i] = -1;
	workIn[0] = 1;
	workIn[2] = 2;
	
	v_opnvwk(workIn, &handle, workOut);
	v_clrwk();
	vq_exit_cur(handle);

	/*
	int16_t pxy[] = {50, 50, 150, 150};
	vs_clip(handle, 1, pxy);

	int16_t rows, cols;
	vq_chcells(handle, &rows, &cols);
	fprintf(stderr, "Rows:%d, cols:%d\n", rows, cols);
	
	v_enter_cur(handle);
	vs_curadress(handle, 20, 10);
	v_curtext(handle, "Hi there?");
	v_rvon(handle);
	v_curtext(handle, "Hey there!");
	v_rvoff(handle);
	
	int16_t row, col;
	vq_curaddress(handle, &row, &col);
	fprintf(stderr, "Cursor at (x=%d, y=%d)", col, row);
	*/
	vsl_type(handle, 3);
	vsl_width(handle, 5);

	srandom(getpid());

	int16_t y  = 50;
	for (int i = SOLID; i<USERLINE; i++)
		{
		vsl_type(handle, i);
		vsl_width(handle, 10);
		vsl_color(handle, random()%16);

		int16_t pts[] = {5, 0, 500, 0};
		pts[1] = pts[3] = y;
		y += 40;
		v_pline(handle, 2, pts);
		}
	
	int16_t pts[] = {5,5, 100,50, 200,5};
	v_pline(handle, 3, pts);
	
	}

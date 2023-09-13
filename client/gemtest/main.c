//
//  main.c
//  gemtest
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <unistd.h>
#include <stdlib.h>
#include <math.h>
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


	int     r = 300;
	int16_t pts[4] = {500, 500, 0, 0};
	vsl_type(handle, SOLID);
	vsl_width(handle, 1);
	vsl_ends(handle, CAP_SQUARE, CAP_ARROW);
	
		for (int j=0; j<360; j+=5)
			{
			int16_t x1 = 500 + r * sin((2*M_PI) * j/360.0);
			int16_t y1 = 500 + r * cos((2*M_PI) * j/360.0);
			pts[2] = x1;
			pts[3] = y1;
			v_pline(handle, 2, pts);
			}
	}

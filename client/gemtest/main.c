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
	
	//int16_t ltrb[] = {200,200,700,600};
	//vs_clip(handle, 1, ltrb);
	srandom(getpid());


	int     r = 300;
	int16_t pts[4] = {500, 500, 0, 0};
	vsm_type(handle, 7);
	vsm_height(handle, 33);
	for (int j=0; j<360; j+=15)
		{
		vsm_color(handle, j%16);
		vsm_type(handle, 1+j%7);
		vsm_height(handle, 20+10*(j%4));
		int16_t x1 = 500 + r * sin((2*M_PI) * j/360.0);
		int16_t y1 = 500 + r * cos((2*M_PI) * j/360.0);
		pts[0] = x1;
		pts[1] = y1;
		v_pmarker(handle, 1, pts);
		}
		
	int num = vst_load_fonts(handle, 0);
	fprintf(stderr, "Loaded %d fonts\n", num);
	
	vst_font(handle, 87);
	vst_effects(handle, TXT_OUTLINE);
	vst_height(handle, 96, NULL, NULL, NULL, NULL);
	v_gtext(handle, 250, 250, "hi there!");
	
	vswr_mode(handle, WR_TRANSPARENT);
	
	int16_t pp[] = {100, 40, 300, 400, 800, 200};
	vsf_color(handle, 11);
	vsf_interior(handle, FIS_PATTERN);
	vsf_style(handle, PT_BALLS);
	vsf_perimeter(handle, 1);
	v_bar(handle, pp);

	vsf_style(handle, PT_UGLY);
	v_pie(handle, 800,400, 100, 0, 900);
	
	vswr_mode(handle, WR_REPLACE);
	vsf_color(handle, 3);
	vsf_style(handle, PT_WEAVE);
	v_circle(handle, 800,400, 50);

	vsf_color(handle, 7);
	vsf_style(handle, PT_CHECKER);
	v_ellipse(handle, 800,400, 25, 50);

	vsf_color(handle, 15);
	vsf_style(handle, PT_DOTS2);
	v_ellpie(handle, 800,700, 100, 50, 0, 900);

	vsl_type(handle, SOLID);
	vsf_color(handle, 3);
	vsf_style(handle, PT_DIAMONDS);
	int16_t rfbpts[] = {50, 600, 150, 700};
	v_rfbox(handle, rfbpts);

	v_arc(handle, 800,600, 100, 0, 1800);
	v_ellarc(handle, 800, 600, 100, 200, 1800, 3600);
	
	vsf_color(handle, 9);
	int16_t rbpts[] = {40, 590, 160, 710};
	v_rbox(handle, rbpts);
	
	vst_height(handle, 12, NULL, NULL, NULL, NULL);
	vsf_interior(handle, FIS_HOLLOW);
	int16_t tpts[] = {995, 495, 1405, 585};
	vsl_color(handle, 1);
	vsl_width(handle, 1);
 	v_bar(handle, tpts);
	
	int16_t rgb[3] = {1000, 100, 500};
	vs_color(handle, 1, rgb);
	vst_alignment(handle, ALGN_LEFT, ALGN_BASELINE, NULL, NULL);
	v_justified(handle, 1000, 500, 400, 0,0, "The quick brown fox jumped over the lazy dog");

	vst_alignment(handle, ALGN_CENTER, ALGN_BASELINE, NULL, NULL);
	v_justified(handle, 1000, 520, 400, 0,0, "The quick brown fox jumped over the lazy dog");

	vst_alignment(handle, ALGN_RIGHT, ALGN_BASELINE, NULL, NULL);
	v_justified(handle, 1000, 540, 400, 0,0, "The quick brown fox jumped over the lazy dog");

	vst_alignment(handle, ALGN_FULL, ALGN_BASELINE, NULL, NULL);
	v_justified(handle, 1000, 560, 400, 0,0, "The quick brown fox jumped over the lazy dog");

	int16_t attrs[10];
	vql_attributes(handle, attrs);
	fprintf(stderr, "type:%d, colour:%d, mode:%d width:%d\n",
			attrs[0],attrs[1], attrs[2], attrs[3]);

	vqm_attributes(handle, attrs);
	fprintf(stderr, "type:%d, colour:%d, mode:%d height:%d\n",
			attrs[0],attrs[1], attrs[2], attrs[3]);

	vqf_attributes(handle, attrs);
	fprintf(stderr, "type:%d, colour:%d, pattern:%d perim:%d\n",
			attrs[0],attrs[1], attrs[2], attrs[3]);

	vqt_attributes(handle, attrs);
	fprintf(stderr, "font:%d, colour:%d, angle:%d, ha:%d, va:%d, mode:%d, w:%d, h:%d\n",
			attrs[0],attrs[1], attrs[2], attrs[3],
			attrs[4],attrs[5], attrs[6], attrs[7]);

	vq_extnd(handle, 1, workOut);
	fprintf(stderr, "Text effect: %x\n", workOut[2]);

//	int16_t mx, my, mb;
//	vrq_locator(handle, 0,0, &mx, &my, &mb);
//	fprintf(stderr, "x=%d, y=%d, btn=%d\n", mx, my, mb);
//
//	char s[1024];
//	vrq_string(handle, 9, 0, NULL, s);
//	fprintf(stderr, "s='%s'\n", s);
	
	vsf_interior(handle, FIS_PATTERN);
	v_contourfill(handle, 1005, 525, 3);
	//int16_t bpts[] = {1005,525,1006,526};
	//v_bar(handle, bpts);
	
	v_clsvwk(handle);
	}

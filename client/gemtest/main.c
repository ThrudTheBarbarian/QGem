//
//  main.c
//  gemtest
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gem.h"
#include "gemio.h"
#include "gemmsg.h"
#include "rscfile.h"
#include "vdi.h"

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }
	

/*****************************************************************************\
|* Test transform to and back again
\*****************************************************************************/
void tstTrans(int w, int wds, int planes,int num)
	{
	uint8_t pbuf[1024];
	for (int i=0; i<1024; i++)
		pbuf[i] = i & 0xff;

	MFDB srcMFDB =
		{
		.fd_addr 	= &(pbuf[0]),
		.fd_stand	= 1,
		.fd_w		= w,
		.fd_h		= 1,
		.fd_wdwidth	= wds,
		.fd_nplanes	= planes
		};

	MFDB dstMFDB =
		{
		.fd_addr = 0,
		.fd_stand = 0
		};

	vr_trnfm(0, &srcMFDB, &dstMFDB);

	MFDB dstMFDB2 =
		{
		.fd_addr = 0,
		.fd_stand = 1
		};
	vr_trnfm(0, &dstMFDB, &dstMFDB2);

	printf("memcmp [%d] = %d\n",
		   planes,
		   memcmp(dstMFDB2.fd_addr, srcMFDB.fd_addr, num));
	}


/*****************************************************************************\
|* Test function for timer callback
\*****************************************************************************/
void tcb(void)
	{
	static int count = 0;
	count ++;
	fprintf(stderr, "%d\n", count);
	}
	
/*****************************************************************************\
|* Program entry point
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

	vsf_udsty(handle, 0xcfcf);
	vsl_type(handle, USERLINE);
	vsf_color(handle, 3);
	vsf_interior(handle, FIS_USER);
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
	
	//int16_t rgb[3] = {1000, 100, 500};
	//vs_color(handle, 1, rgb);
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
	
	vsf_interior(handle, FIS_SOLID);
	vsf_color(handle, 2);
//	v_contourfill(handle, 1005, 525, 1);
	//int16_t bpts[] = {1005,525,1006,526};
	//v_bar(handle, bpts);
	
//	int16_t rgb = 0;
//	v_get_pixel(handle, 1005, 525, &rgb, NULL);
//	fprintf(stderr, "pix: %0x\n", rgb);
//
//	int16_t prgb[3] = {0,128,64};
//	for (int i=0; i<255; i++)
//		{
//		prgb[1] = i;
//		for (int j=0; j<255; j++)
//			{
//			prgb[2] = j;
//			vs_pixrgb(handle, i, j, prgb);
//			}
//		}


//	MFDB srcMFDB =
//		{
//		.fd_addr = 0,
//		.fd_stand = 0
//		};
//
//	MFDB dstMFDB =
//		{
//		.fd_addr = 0,
//		.fd_stand = 0
//		};
//
//	int16_t pxyblit[8] = {0, 0, 400, 400, 400, 0, 400, 400};
//	vro_cpyfm(handle, S_XOR_D, pxyblit, &srcMFDB, &dstMFDB);
	
	
	tstTrans(32, 4, 4, 16);
	tstTrans(32, 2, 8, 32);

	RscFile colourIcons;
	if (resourceLoad("cicons.rsc", &colourIcons))
 		{
		MFDB src;
		//src.fd_addr 	= colourIcons.cIcons[29].icons[1].colData;
		src.fd_addr 	= colourIcons.cIcons[29].monoIcon.ib_pdata;
		src.fd_w		= colourIcons.cIcons[29].monoIcon.ib_wicon;
		src.fd_h		= colourIcons.cIcons[29].monoIcon.ib_hicon;
		src.fd_wdwidth	= ((src.fd_w) / 16) + ((src.fd_w & 15) != 0 ? 1 : 0);
		src.fd_stand	= MFDB_STANDARD;
		//src.fd_nplanes	= colourIcons.cIcons[29].icons[1].numPlanes;
		src.fd_nplanes	= 1;
		src.fd_r1		= 0;
		src.fd_r2		= 0;
		src.fd_r3		= 0;

		MFDB dst;
		memset(&dst, 0, sizeof(MFDB));
		MFDB screen = dst;

		vr_trnfm(handle, &src, &dst);
		int px = 600;
		int py = 300;
		int16_t pxy[8]	=
			{
			0, 0,
			src.fd_w-1, src.fd_h-1,
			px, py,
			px+src.fd_w*4, py+src.fd_h*4,
			};

		//int16_t mode = ((int16_t)0x8000) | S_ONLY;
		//vro_cpyfm(handle, mode, pxy, &dst, &screen);
		
		int16_t cols[2] = {1,0};
		int16_t mode = ((int16_t)0x8000) | WR_REV_TRANS;
		vrt_cpyfm(handle, mode, pxy, &dst, &screen, cols);
		}
	else
		fprintf(stderr, "Failed to load colour icons\n");



//	int16_t cdata[] =
//		{
//		0x5555, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc,
//		0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x0000,
//		0xffff, 0x8ff1, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001,
//		0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xffff,
//		0x0000, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc,
//		0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x3ffc, 0x0000,
//		0xffff, 0x8ff1, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001,
//		0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xffff,
//		};
//	vsf_udpat(handle, cdata, 4);
	
	int16_t mode;
	vsin_mode(handle, INP_LOCATOR, INPUT_REQUEST);
	vqin_mode(handle, INP_LOCATOR, &mode);
	fprintf(stderr, "SetInpReq  : %s\n",
			((mode == INPUT_REQUEST) ? ("ok") : ("failed")));
	
	vsin_mode(handle, INP_LOCATOR, INPUT_SAMPLE);
	vqin_mode(handle, INP_LOCATOR, &mode);
	fprintf(stderr, "SetInpReq  : %s\n",
			(mode == INPUT_SAMPLE) ? ("ok") : "failed");
	
	
	int16_t extents[8];
	vst_rotation(handle, 0);
	vst_height(handle, 10, NULL, NULL, NULL, NULL);
	vqt_extent(handle, "Hi there", extents);
	for(int i=0; i<8; i+=2)
		fprintf(stderr, "[%d,%d] ", extents[i], extents[i+1]);
	fprintf(stderr, "\n");
	
	vst_rotation(handle, 900);
	vst_height(handle, 10, NULL, NULL, NULL, NULL);
	vqt_extent(handle, "Hi there", extents);
	for(int i=0; i<8; i+=2)
		fprintf(stderr, "[%d,%d] ", extents[i], extents[i+1]);
	fprintf(stderr, "\n");
	
	
	int16_t cw, ld, rd;
	vqt_width(handle, 'X', &cw, &ld, &rd);
	fprintf(stderr, "X: %d, %d, %d\n", cw, ld, rd);
	
//	int16_t ms;
//	vex_timv(handle, tcb, NULL, &ms);
//	fprintf(stderr, "ms=%d\n", ms);
//	
//	GemMsg msg;
//	_gemIoWaitForMessageOfType(&msg, MSG_REPLY(MSG_VQT_WIDTH));
	
	//v_hide_c(handle);
	//v_show_c(handle, 1);
	
	v_clsvwk(handle);
	}
	

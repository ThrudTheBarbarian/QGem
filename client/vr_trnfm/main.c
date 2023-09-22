//
//  main.c
//  vr_trnfm
//
//  Created by Simon Gornall on 9/20/23.
//

#include <stdio.h>
#include <string.h>

#include "gem.h"
#include "vdi.h"


int debugLevel(void)
	{ return 10; }


/*****************************************************************************\
|* Dump out an MFDB and its contents - just a debugging tool
\*****************************************************************************/
void dump(MFDB *mfdb)
	{
	printf("MFDB:\n width    : %d\n"
	              " height   : %d\n"
	              " words/ln : %d\n"
	              " standard : %s\n"
	              " planes   : %d\n"
	              " reserved : %04x,%04x,%04x\n\n",
	              mfdb->fd_w,
	              mfdb->fd_h,
	              mfdb->fd_wdwidth,
	              (mfdb->fd_stand == 0) ? ("device") : ("standard"),
	              mfdb->fd_nplanes,
	              mfdb->fd_r1, mfdb->fd_r2, mfdb->fd_r3);
	              
	uint16_t *ptr 	= (uint16_t *)mfdb->fd_addr;
	int planeOffset = (mfdb->fd_wdwidth) * mfdb->fd_h;
	for (int i=0; i<mfdb->fd_nplanes; i++)
		{
		printf("Plane %02d @ %04x:\7", i, planeOffset * i * 2);
		uint16_t *data = ptr + planeOffset * i;
		int words = mfdb->fd_wdwidth;
		for (int j=0; j<mfdb->fd_h; j++)
			for (int k=0; k<words; k++)
				printf("%04x ", *data++);
		printf("\n");
		}
	}

/*****************************************************************************\
|* Test 8-bit unindexed (ie gray-level) to planar
\*****************************************************************************/
void convPlanarGreyscale8(void)
	{
	uint16_t expected[] = {0x8080, 0x8000, 0xc080, 0xc080,
						   0xe080, 0xe080, 0xf080, 0xf080,
						   0xf880, 0xf880, 0xfc80, 0xfc80,
						   0xfe80, 0xfe80, 0xff80, 0xff80};
						
	char img9x2[] 		= {0xff, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1, 0xff, 0,
						   0xff, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1, 0x7f, 0};
	MFDB chunky;
	chunky.fd_addr 		= img9x2;
	chunky.fd_w			= 9;
	chunky.fd_h			= 2;
	chunky.fd_wdwidth	= 5;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 8;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 0;
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	
	vr_trnfm(0, &chunky, &dst);
	
	
	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 8-bit unindexed   : %s",
		(memcmp(ptr, expected, 32) != 0) ? ("FAIL\n") : ("pass\n"));
	}

/*****************************************************************************\
|* Test 16-bit RGB to planar
\*****************************************************************************/
void convPlanar16(void)
	{
	uint16_t expected[] = {0x8000, 0xc000, 0xe000, 0xf000,
						   0xf800, 0xfc00, 0xfe00, 0xff00,
						   0x0080, 0x0080, 0x0080, 0x0080,
						   0x0080, 0x0080, 0x0080, 0x0080};
						
	uint16_t img9x1[] 	= {0xff00, 0x7f00, 0x3f00, 0x1f00,
						   0x0f00, 0x0700, 0x0300, 0x0100,
						   0x00ff};
	MFDB chunky;
	chunky.fd_addr 		= img9x1;
	chunky.fd_w			= 9;
	chunky.fd_h			= 1;
	chunky.fd_wdwidth	= 9;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 16;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 0;
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	
	vr_trnfm(0, &chunky, &dst);
	uint16_t *ptr = (uint16_t *)dst.fd_addr;

	printf(" - 16-bit RGB        : %s",
		(memcmp(ptr, expected, 32) != 0) ? ("FAIL\n") : ("pass\n"));
	}


/*****************************************************************************\
|* Test 24-bit RGB to planar
\*****************************************************************************/
void convPlanar24(void)
	{
	uint16_t expected[] = {
							0x4000, 0x1000, 0x4000, 0x1000,
							0x4000, 0x1000, 0x4000, 0x1000,
							0x4000, 0x1000, 0x4000, 0x1000,
							0x4000, 0x1000, 0x4000, 0x1000,
							0x2000, 0x4000, 0x2000, 0x4000,
							0x2000, 0x4000, 0x2000, 0x4000,
							0x2000, 0x4000, 0x2000, 0x4000,
							0x2000, 0x4000, 0x2000, 0x4000,
							0x1000, 0x2000, 0x1000, 0x2000,
							0x1000, 0x2000, 0x1000, 0x2000,
							0x1000, 0x2000, 0x1000, 0x2000,
							0x1000, 0x2000, 0x1000, 0x2000,
							};
							
	// 0xff0000, 0x00ff00, 0x0000ff
	// 0x00ff00, 0x0000ff, 0xff0000
	
	uint8_t img3x2[] 	= {	0xff, 0x00, 0x00,
							0x00, 0xff, 0x00,
							0x00, 0x00, 0xff, 0x00,
							0x00, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0x00, 0x00, 0x00};
	MFDB chunky;
	chunky.fd_addr 		= img3x2;
	chunky.fd_w			= 3;
	chunky.fd_h			= 2;
	chunky.fd_wdwidth	= 5;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 24;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 0;
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	
	vr_trnfm(0, &chunky, &dst);
	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 24-bit RGB        : %s",
		(memcmp(ptr, expected, 48) != 0) ? ("FAIL\n") : ("pass\n"));
	}

/*****************************************************************************\
|* Test 2-bit packed to planar
\*****************************************************************************/
void convPlanar2(void)
	{
	uint16_t expected[] = {
							0x00c4, 0x0038, 0x00a0, 0x005c,
							
							// RGB
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00
							};
							
	// 11.10.01.00 00.10 [00.00]
	// 00.01.10.11 11.01 [00.00]
	
	uint8_t img6x2[] 	= {	0xE4, 0x20,
							0x1B, 0xD0,
							// RGB data
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff
	};
	MFDB chunky;
	chunky.fd_addr 		= img6x2;
	chunky.fd_w			= 6;
	chunky.fd_h			= 2;
	chunky.fd_wdwidth	= 1;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 2;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 4;
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	vr_trnfm(0, &chunky, &dst);
	
	//dump(&dst);
	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 2-bit indexed     : %s",
		(memcmp(ptr, expected, 20) != 0) ? ("FAIL\n") : ("pass\n"));
	
	}

/*****************************************************************************\
|* Test 4-bit packed to planar
\*****************************************************************************/
void convPlanar4(void)
	{
	uint16_t expected[] = {
							0x00ff, 0x0000, 0x00f0, 0x000f,
							0x00cc, 0x0033, 0x00aa, 0x0055,
							
							// RGB
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00
							};
							
	
	uint8_t img8x2[] 	= {	0xFE, 0xDC, 0xBA, 0x98,
							0x01, 0x23, 0x45, 0x67,
							// RGB data
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff
	};
	MFDB chunky;
	chunky.fd_addr 		= img8x2;
	chunky.fd_w			= 8;
	chunky.fd_h			= 2;
	chunky.fd_wdwidth	= 2;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 4;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 16;
	
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	vr_trnfm(0, &chunky, &dst);

	//dump(&dst);

	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 4-bit indexed     : %s",
		(memcmp(ptr, expected, 64) != 0) ? ("FAIL\n") : ("pass\n"));
	
	}


/*****************************************************************************\
|* Test 8-bit packed to planar
\*****************************************************************************/
void convPlanar8(void)
	{
	uint16_t expected[] = {
							0xf000, 0x0000, 0xc000, 0x3000,
							0xa000, 0x5000, 0xf000, 0x0000,
							0xf000, 0x0000, 0xc000, 0x3000,
							0xa000, 0x5000, 0x0000, 0xf000,
							
							// RGB
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00
							};
							
	
	uint8_t img4x2[] 	= {	0xFE, 0xDC, 0xBA, 0x98,
							0x01, 0x23, 0x45, 0x67,
							// RGB data
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff
	};
	MFDB chunky;
	chunky.fd_addr 		= img4x2;
	chunky.fd_w			= 4;
	chunky.fd_h			= 2;
	chunky.fd_wdwidth	= 2;
	chunky.fd_stand		= MFDB_DEVICE;
	chunky.fd_nplanes	= 8;
	chunky.fd_r1		= 0;
	chunky.fd_r2		= 0;
	chunky.fd_r3		= 16;
	
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	vr_trnfm(0, &chunky, &dst);

	//dump(&dst);
	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 8-bit indexed     : %s",
		(memcmp(ptr, expected, 78) != 0) ? ("FAIL\n") : ("pass\n"));
	
	}



/*****************************************************************************\
|* Test 2-bit planar to chunky
\*****************************************************************************/
void convChunky2(void)
	{
	uint16_t expected[] = {
							0x000f, 0x0000, 0x000c, 0x0003,
							0x000a, 0x0005, 0x000f, 0x0000,
							0x000f, 0x0000, 0x000c, 0x0003,
							0x000a, 0x0005, 0x0000, 0x000f,
							
							// RGB
							0xffff, 0x00ff, 0x0000, 0xffff,
							0x0000, 0xff00,
							};
							
	
	uint8_t img4x2[] 	= {	0xF0, 0x00, 0xF0, 0x00,
							0x0F, 0x00, 0x0F, 0x00,
							// RGB data
							0xff, 0xff, 0xff,
							0x00, 0x00, 0x00,
							0xff, 0xff, 0x00,
							0x00, 0x00, 0xff,
							};
	MFDB planar;
	planar.fd_addr 		= img4x2;
	planar.fd_w			= 4;
	planar.fd_h			= 2;
	planar.fd_wdwidth	= 1;
	planar.fd_stand		= MFDB_STANDARD;
	planar.fd_nplanes	= 2;
	planar.fd_r1		= 0;
	planar.fd_r2		= 0;
	planar.fd_r3		= 4;
	
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	vr_trnfm(0, &planar, &dst);

	dump(&dst);

	uint16_t *ptr = (uint16_t *)dst.fd_addr;
	printf(" - 2-bit indexed     : %s",
		(memcmp(ptr, expected, 78) != 0) ? ("FAIL\n") : ("pass\n"));
	
	}

/*****************************************************************************\
|* Program entry point
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	printf("chunky to planar:\n");
	convPlanar2();
	convPlanar4();
	convPlanar8();
	convPlanarGreyscale8();
	convPlanar16();
	convPlanar24();

	printf("\nplanar to chunky:\n");
	convChunky2();
	
	printf("\n\n");
	}

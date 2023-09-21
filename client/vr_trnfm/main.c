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

int main(int argc, const char * argv[])
	{
	printf("chunky to planar:\n");
	convPlanarGreyscale8();
	convPlanar16();
	convPlanar24();
	}

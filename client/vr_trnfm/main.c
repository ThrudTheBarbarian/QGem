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

int main(int argc, const char * argv[])
	{

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
	
	int planeOffset = (dst.fd_wdwidth);
	for (int i=0; i<8; i++)
		{
		printf("Plane %d @ %04x:\7", i, planeOffset * i);
		uint16_t *data = ptr + planeOffset * i;
		int words = dst.fd_wdwidth;
		for (int j=0; j<words; j++)
			printf("%04x ", *data++);
		printf("\n");
		}
	}

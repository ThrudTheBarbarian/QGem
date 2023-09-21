//
//  vr_trnfm.c
//  gemc
//
//  Created by ThrudTheBarbarian on 9/20/23.
//

#include <stdio.h>

#include "debug.h"
#include "gem.h"
#include "macros.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static void _convPlanarIndexed8(MFDB *src, MFDB *dst);
static void _convPlanar16(MFDB *src, MFDB *dst);
static void _convPlanar24(MFDB *src, MFDB *dst);
static void _convPlanarGreyscale8(MFDB *src, MFDB *dst);
static void _convPlanar16(MFDB *src, MFDB *dst);


/*****************************************************************************\
|*  110  : Transform an image from planar to chunky or vice-versa.
|*
|* If a planar image is:
|*   1-bit	: leave it as 1-bit and allow the server to construct
|*	 2-bit	: convert to 8-bit indexed
|*	 4-bit	: convert to 8-bit indexed
|* 	 8-bit	: convert to 8-bit indexed
|*	16-bit	: convert to 16-bit RRRRRggggggBBBBB
|*	24-bit	: convert to 24-bit RGB
|*
|* o For 8-bit indexed, the RGB values are appended to the allocated data and
|*   the entry count is placed in the 3rd reserved MFDB value to indicate
|*   the colourmap is present. If this is 0, greyscale is assumed.
|* o if dst->fd_addr is NULL, it will be allocated
|* o if dst->fd_addr is NOT null, it will be reallocated (ie: free/realloc)
|*   if 8-bit indexed is used, so that the colourmap can be incorporated
|*
|* If a chunky image is:
|* 	 1-bit : leave as 1-bi
|* 	 8-bit : examine size of colourmap and fit into 2,4,or 8-bit planes
|* 	16-bit : convert to 16-bit planes
|*	24-bit	: convert to 24-bit planes
|*
|* o if dst->fd_addr is NULL, it will be allocated
|* o if dst->fd_addr is NOT null, it will be used, and must contain sufficient
|*   space for the data.
\*****************************************************************************/
void vr_trnfm(int16_t handle, MFDB *src, MFDB *dst)
	{
	/*************************************************************************\
	|* Check we don't have NULL pointers
	\*************************************************************************/
	if ((src == NULL) || (dst == NULL))
		{
		WARN("src [%p] / dst [%p] passed as NULL", src, dst);
		return;
		}
		
	/*************************************************************************\
	|* If we're going from device -> "standard" ...
	\*************************************************************************/
	if (src->fd_stand == 0)
		{
		switch (src->fd_nplanes)
			{
			case 1:
				/* do nothing */
				break;
			case 8:
				_convPlanarIndexed8(src, dst);
				break;
			case 16:
				_convPlanar16(src, dst);
				break;
			case 24:
				//_convPlanar24(src, dst);
				break;
			default:
				WARN("Unknown sourcce bitmap format %d in vr_trnfm",
						src->fd_nplanes);
				break;
			}
		}
	else
		{
		}
	}

/*****************************************************************************\
|* Convert 8-bit indexed to planar
\*****************************************************************************/
static void _convPlanarIndexed8(MFDB *src, MFDB *dst)
	{
	switch (src->fd_r3)
		{
		case 0:
			/* greyscale */
			_convPlanarGreyscale8(src, dst);
			break;
		case 2:
			//_convPlanar2(src, dst);
			break;
		case 4:
			//_convPlanar4(src, dst);
			break;
		case 8:
			//_convPlanar8(src, dst);
			break;
		default:
			WARN("Unsupported bit depth %d in vr_trnfm", src->fd_r3);
			break;
		}
	}

/*****************************************************************************\
|* Convert 8-bit greyscale to planar
\*****************************************************************************/
static void _convPlanarGreyscale8(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	/*************************************************************************\
	|* Each plane is 8-bits in size
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 8 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 8 * H * 2;
	
	/*************************************************************************\
	|* If the destination MFDB has a NULL data reference, alloc memory
	\*************************************************************************/
	if (dst->fd_addr == NULL)
		{
		dst->fd_addr = malloc(bytesRequired);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return;
			}
		}
	
	/*************************************************************************\
	|* If the src and dest are the same, alloc and copy it over later
	\*************************************************************************/
	int needsCopy = 0;
	if (src->fd_addr == dst->fd_addr)
		{
		dst->fd_addr = malloc(bytesRequired);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return;
			}
		needsCopy = 1;
		}
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[8];
	for (int i=0; i<8; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr) + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Figure out if there is a skip at the end of the source MFDB lines
	\*************************************************************************/
	int delta = src->fd_wdwidth*2 - src->fd_w;

	/*************************************************************************\
	|* Run over the source data, planifying
	\*************************************************************************/
	uint8_t *data = src->fd_addr;
	for (int i=0; i<H; i++)
		{
		/*********************************************************************\
		|* For each line...
		\*********************************************************************/
		for (int j=0; j<W;)
			{
			/*****************************************************************\
			|* Is this a full 16-bits, or whatever we have left over
			\*****************************************************************/
			int loops 	= MIN(8, W-j);
			int rotate	= 16;
			
			uint16_t p0 = 0, p1 = 0, p2 = 0, p3 = 0;
			uint16_t p4 = 0, p5 = 0, p6 = 0, p7 = 0;
			
			/*****************************************************************\
			|* Read 'loops' bytes of the line, and disperse over the 8 planes
			\*****************************************************************/
			uint8_t set = 0x80;
			for (int k=0; k<loops; k++)
				{
				uint8_t bit 	= 0x80;
				uint8_t val 	= *data ++;
				
				p0 |= ((val & bit) ? set : 0); bit >>=1;
				p1 |= ((val & bit) ? set : 0); bit >>=1;
				p2 |= ((val & bit) ? set : 0); bit >>=1;
				p3 |= ((val & bit) ? set : 0); bit >>=1;
				p4 |= ((val & bit) ? set : 0); bit >>=1;
				p5 |= ((val & bit) ? set : 0); bit >>=1;
				p6 |= ((val & bit) ? set : 0); bit >>=1;
				p7 |= ((val & bit) ? set : 0); bit >>=1;
				set >>= 1;
				rotate --;
				}
			j += loops;
			
			if (loops == 8)
				{
				loops  = MIN(8, W-j);
				
				/*************************************************************\
				|* Read 'loops' more bytes, and disperse over the 8 planes
				\*************************************************************/
				for (int k=0; k<loops; k++)
					{
					uint8_t bit 	= 0x80;
					uint8_t val 	= *data ++;
					
					p0 = (val & bit) ? (p0 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p1 = (val & bit) ? (p1 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p2 = (val & bit) ? (p2 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p3 = (val & bit) ? (p3 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p4 = (val & bit) ? (p4 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p5 = (val & bit) ? (p5 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p6 = (val & bit) ? (p6 << 1) | 1 : (p0 << 1);	bit >>= 1;
					p7 = (val & bit) ? (p7 << 1) | 1 : (p0 << 1);
					rotate --;
					}
				j += loops;
				}
			
			/*****************************************************************\
			|* Update all the plane data
			\*****************************************************************/
			*ptr[0] ++ = p0 << rotate;
			*ptr[1] ++ = p1 << rotate;
			*ptr[2] ++ = p2 << rotate;
			*ptr[3] ++ = p3 << rotate;
			*ptr[4] ++ = p4 << rotate;
			*ptr[5] ++ = p5 << rotate;
			*ptr[6] ++ = p6 << rotate;
			*ptr[7] ++ = p7 << rotate;
			}
			
		/*********************************************************************\
		|* Add in any offset in the source so we're at the start of the line
		\*********************************************************************/
		data += delta;
		}
	
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, W*H);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_STANDARD;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerPlanePerLine  * dst->fd_h;
	dst->fd_nplanes	= 8;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= 0;
	}

/*****************************************************************************\
|* Convert 16-bit RGB to planar
\*****************************************************************************/
static void _convPlanar16(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	if (dst->fd_addr == NULL)
		{
		dst->fd_addr = malloc(W*H*2);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return;
			}
		}
	
	int needsCopy = 0;
	if (src->fd_addr == dst->fd_addr)
		{
		dst->fd_addr = malloc(W*H*2);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return;
			}
		needsCopy = 1;
		}
		
	uint8_t *ptr[16];
	for (int i=0; i<16; i++)
		ptr[i] = dst->fd_addr + i * ((W*H)/8);
	
	uint16_t *data = src->fd_addr;
	for (int i=0; i<H; i++)
		{
		for (int j=0; j<W;)
			{
			int loops = MIN(8, W-j);
			uint8_t p0 = 0, p1 = 0, p2 = 0, p3 = 0;
			uint8_t p4 = 0, p5 = 0, p6 = 0, p7 = 0;
			
			for (int k=0; k<loops; k++)
				{
				uint16_t val = *data ++;
				p0 = (p0 << 1) | (val & 1); val >>= 1;
				p1 = (p1 << 1) | (val & 1); val >>= 1;
				p2 = (p2 << 1) | (val & 1); val >>= 1;
				p3 = (p3 << 1) | (val & 1); val >>= 1;
				p4 = (p4 << 1) | (val & 1); val >>= 1;
				p5 = (p5 << 1) | (val & 1); val >>= 1;
				p6 = (p6 << 1) | (val & 1); val >>= 1;
				p7 = (p7 << 1) | (val & 1); val >>= 1;
				}
			j += loops;
			
			*ptr[0] ++ = p0;
			*ptr[1] ++ = p1;
			*ptr[2] ++ = p2;
			*ptr[3] ++ = p3;
			*ptr[4] ++ = p4;
			*ptr[5] ++ = p5;
			*ptr[6] ++ = p6;
			*ptr[7] ++ = p7;
			}
		}
	
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, W*H);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand = MFDB_STANDARD;
	}

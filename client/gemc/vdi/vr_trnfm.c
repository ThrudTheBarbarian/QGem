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
static void _convPlanar24(MFDB *src, MFDB *dst);


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
				_convPlanar24(src, dst);
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
			/* greyscale or unindexed */
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
|* Handle memory allocation
\*****************************************************************************/
static int _allocMFDB(MFDB *src, MFDB *dst, int bytesRequired)
	{
	int needsCopy = 0;
	
	/*************************************************************************\
	|* If the destination MFDB has a NULL data reference, alloc memory
	\*************************************************************************/
	if (dst->fd_addr == NULL)
		{
		dst->fd_addr = malloc(bytesRequired);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return -1;
			}
		}
	
	/*************************************************************************\
	|* If the src and dest are the same, alloc and copy it over later
	\*************************************************************************/
	if (src->fd_addr == dst->fd_addr)
		{
		dst->fd_addr = malloc(bytesRequired);
		if (dst->fd_addr == NULL)
			{
			WARN("Failed to allocate space for conversion in vr_trnfm");
			return -1;
			}
		needsCopy = 1;
		}
		
	return needsCopy;
	}
	
/*****************************************************************************\
|* Convert 8-bit greyscale to planar
\*****************************************************************************/
static void _convPlanarGreyscale8(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	/*************************************************************************\
	|* Each plane is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 8 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 8 * H * 2;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[8];
	for (int i=0; i<8; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Figure out if there is a skip at the end of the source MFDB lines
	\*************************************************************************/
	int delta = src->fd_wdwidth*2 - src->fd_w;

	/*************************************************************************\
	|* Run over the source data, plane-ifying
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
			|* Is this a full 16-bits, or just whatever we have left over
			\*****************************************************************/
			int loops 	= MIN(16, W-j);
			int shift	= 16;
			
			uint16_t p0 = 0, p1 = 0, p2 = 0, p3 = 0;
			uint16_t p4 = 0, p5 = 0, p6 = 0, p7 = 0;
			
			/*****************************************************************\
			|* Read 'loops' bytes of the line, and disperse over the 8 planes
			\*****************************************************************/
			for (int k=0; k<loops; k++)
				{
				uint8_t bit 	= 0x80;
				uint8_t val 	= *data ++;
				
				p0 <<= 1; if (val & bit) p0 |= 1; bit >>= 1;
				p1 <<= 1; if (val & bit) p1 |= 1; bit >>= 1;
				p2 <<= 1; if (val & bit) p2 |= 1; bit >>= 1;
				p3 <<= 1; if (val & bit) p3 |= 1; bit >>= 1;
				p4 <<= 1; if (val & bit) p4 |= 1; bit >>= 1;
				p5 <<= 1; if (val & bit) p5 |= 1; bit >>= 1;
				p6 <<= 1; if (val & bit) p6 |= 1; bit >>= 1;
				p7 <<= 1; if (val & bit) p7 |= 1;
				
				if (loops == 7)
					{
					bit 	= 0x80;
					val 	= *data ++;
					}
				shift --;
				}
			j += loops;
			
			/*****************************************************************\
			|* Update all the plane data
			\*****************************************************************/
			*ptr[0] ++ = p0 << shift;
			*ptr[1] ++ = p1 << shift;
			*ptr[2] ++ = p2 << shift;
			*ptr[3] ++ = p3 << shift;
			*ptr[4] ++ = p4 << shift;
			*ptr[5] ++ = p5 << shift;
			*ptr[6] ++ = p6 << shift;
			*ptr[7] ++ = p7 << shift;
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
	dst->fd_wdwidth	= wordsPerPlanePerLine;
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
	
	/*************************************************************************\
	|* Each plane is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 16 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 16 * H * 2;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[16];
	for (int i=0; i<16; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Run over the source data, plane-ifying
	\*************************************************************************/
	uint16_t *data = src->fd_addr;
	for (int i=0; i<H; i++)
		{
		/*********************************************************************\
		|* For each line...
		\*********************************************************************/
		for (int j=0; j<W;)
			{
			/*****************************************************************\
			|* Is this a full 16-pixels, or just whatever we have left over
			\*****************************************************************/
			int loops 	= MIN(16, W-j);
			int shift	= 16;
			
			uint16_t p[16];
			memset(p, 0, 32);
			
			/*****************************************************************\
			|* Read 'loops' bytes of the line, and disperse over the 8 planes
			\*****************************************************************/
			for (int k=0; k<loops; k++)
				{
				uint16_t bit 	= 0x8000;
				uint16_t val 	= *data ++;
				
				for (int l=0; l<16; l++)
					{
					p[l] <<= 1;
					if (val & bit)
						p[l] |= 1;
					bit >>= 1;
					}
				
				shift --;
				}
			j += loops;
			
			/*****************************************************************\
			|* Update all the plane data
			\*****************************************************************/
			for (int i=0; i<16; i++)
				*ptr[i] ++ = p[i] << shift;
			}
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
	dst->fd_wdwidth	= wordsPerPlanePerLine;
	dst->fd_nplanes	= 16;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= 0;
	}
	
/*****************************************************************************\
|* Convert 24-bit RGB to planar
\*****************************************************************************/
static void _convPlanar24(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	/*************************************************************************\
	|* Each plane is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 24 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 24 * H * 2;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[24];
	for (int i=0; i<24; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Figure out if there is a skip at the end of the source MFDB lines
	\*************************************************************************/
	int delta = src->fd_wdwidth*2 - src->fd_w*3;

	/*************************************************************************\
	|* Run over the source data, plane-ifying
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
			|* Is this a full 16-pixels, or just whatever we have left over
			\*****************************************************************/
			int loops 	= MIN(24, W-j);
			int shift	= 15;
			
			uint16_t p[24];
			memset(p, 0, 48);
			
			/*****************************************************************\
			|* Read 'loops' bytes of the line, and disperse over the 8 planes
			\*****************************************************************/
			for (int k=0; k<loops; k++)
				{
				uint32_t bit 	= 0x800000;
				uint32_t val 	= *data ++;
				val				= (val << 8) | *data ++;
				val				= (val << 8) | *data ++;
				
				for (int l=0; l<24; l++)
					{
					p[l] <<= 1;
					if (val & bit)
						p[l] |= 1;
					bit >>= 1;
					}
				
				shift --;
				}
			j += loops;
			
			/*****************************************************************\
			|* Update all the plane data
			\*****************************************************************/
			for (int i=0; i<24; i++)
				*ptr[i] ++ = p[i] << shift;
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
	dst->fd_wdwidth	= wordsPerPlanePerLine;
	dst->fd_nplanes	= 24;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= 0;
	}

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
static void _convToPlanar16(MFDB *src, MFDB *dst);
static void _convToPlanar24(MFDB *src, MFDB *dst);
static void _convToPlanar2(MFDB *src, MFDB *dst);
static void _convToPlanar16(MFDB *src, MFDB *dst);
static void _convToPlanar24(MFDB *src, MFDB *dst);
static void _convToPlanar4(MFDB *src, MFDB *dst);
static void _convToPlanar8(MFDB *src, MFDB *dst);

static void _convToChunky2(MFDB *src, MFDB *dst);
static void _convToChunky4(MFDB *src, MFDB *dst);
static void _convToChunky8(MFDB *src, MFDB *dst);
static void _convToChunky16(MFDB *src, MFDB *dst);
static void _convToChunky24(MFDB *src, MFDB *dst);

#pragma mark - Entry point

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
|* 	 1-bit : leave as 1-bit
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
	if (src->fd_stand == MFDB_DEVICE)
		{
		switch (src->fd_nplanes)
			{
			case 1:
				/* copy src to dst */
				if (dst->fd_addr !=NULL)
					free(dst->fd_addr);
				memcpy(dst, src, sizeof(MFDB));
				dst->fd_stand = MFDB_STANDARD;
			case 2:
				_convToPlanar2(src, dst);
				break;
			case 4:
				_convToPlanar4(src, dst);
				break;
			case 8:
				_convToPlanar8(src, dst);
				break;
			case 16:
				_convToPlanar16(src, dst);
				break;
			case 24:
				_convToPlanar24(src, dst);
				break;
			default:
				WARN("Unknown source bitmap format %d in vr_trnfm",
						src->fd_nplanes);
				break;
			}
		}

	/*************************************************************************\
	|* else "standard => device ...
	\*************************************************************************/
	else
		{
		switch (src->fd_nplanes)
			{
			case 1:
				/* copy src to dst */
				if (dst->fd_addr !=NULL)
					free(dst->fd_addr);
				memcpy(dst, src, sizeof(MFDB));
				dst->fd_stand = MFDB_DEVICE;
				break;
			case 2:
				_convToChunky2(src, dst);
				break;
			case 4:
				_convToChunky4(src, dst);
				break;
			case 8:
				_convToChunky8(src, dst);
				break;
			case 16:
				_convToChunky16(src, dst);
				break;
			case 24:
				_convToChunky24(src, dst);
				break;
			default:
				WARN("Unknown source bitmap format %d in vr_trnfm",
						src->fd_nplanes);
				break;
			}
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

#pragma mark - chunky -> planar
	
/*****************************************************************************\
|* Convert 2-bit indexed to planar
\*****************************************************************************/
static void _convToPlanar2(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	
	/*************************************************************************\
	|* Each plane in dst is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 2 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 2 * H * 2;

	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[2];
	for (int i=0; i<2; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Run over the source data, plane-ifying
	\*************************************************************************/
	uint8_t *data = src->fd_addr;
	uint16_t p[2] = {0,0};						// Aggregated plane values
	for (int i=0; i<H; i++)
		{
		/*********************************************************************\
		|* For each line of words...
		\*********************************************************************/
		int words = src->fd_wdwidth;
		
		for (int j=0; j<words; j++)
			{
			/*****************************************************************\
			|* Read the next word and decompose into pixel values
			\*****************************************************************/
			uint16_t val 	= *data ++;
			val				= (val << 8) | *data ++;
			
			/*****************************************************************\
			|* For each pixel, disperse over the 2 planes
			\*****************************************************************/
			int pixPerWord 	= 8;				// 8 pixels per 16-bit word
			int pixMask		= 0xC000;			// Mask for the pixel extract
			int pixShift	= 14;				// shift pixel to low 2 bits
							
			for (int i=0; i<pixPerWord; i++)
				{
				int pixel	= (val & pixMask) >> pixShift;
				pixMask   >>= 2;
				pixShift   -= 2;
				
				p[0] 		= (p[0] << 1) | (pixel >> 1);
				p[1]		= (p[1] << 1) | (pixel & 1);;
				}
			
			/*****************************************************************\
			|* If this is an 'odd' pixel, then store the aggregated values
			|* then clear them
			\*****************************************************************/
			if (((j&1) == 1) || (j == words-1))
				{
				for (int i=0; i<2; i++)
					*ptr[i] ++ = p[i];
				p[0] = p[1] = 0;
				}
			}
		}
	
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		uint8_t * srcRgb = data;
		uint8_t * dstRgb = (uint8_t *)ptr[1];
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_STANDARD;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerPlanePerLine;
	dst->fd_nplanes	= 2;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 4-bit indexed to planar
\*****************************************************************************/
static void _convToPlanar4(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	
	/*************************************************************************\
	|* Each plane in dst is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 4 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 4 * H * 2;

	/*************************************************************************\
	|* Add on space for the RGB values for each of the 16 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the per-plane data
	\*************************************************************************/
	uint16_t *ptr[4];
	for (int i=0; i<4; i++)
		ptr[i] = (uint16_t *)(dst->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Run over the source data, plane-ifying
	\*************************************************************************/
	uint8_t *data = src->fd_addr;
	uint16_t p[4] = {0,0,0,0};					// Aggregated plane values
	for (int i=0; i<H; i++)
		{
		/*********************************************************************\
		|* For each line of words...
		\*********************************************************************/
		int words = src->fd_wdwidth;
		
		int bitsToProcess = 1;
		for (int j=0; j<words; j++)
			{
			bitsToProcess = 1;
			
			/*****************************************************************\
			|* Read the next word and decompose into pixel values
			\*****************************************************************/
			uint16_t val 	= *data ++;
			val				= (val << 8) | *data ++;
			
			/*****************************************************************\
			|* For each pixel, disperse over the 2 planes
			\*****************************************************************/
			int pixPerWord 	= 4;				// 4 pixels per 16-bit word
			int pixMask		= 0xF000;			// Mask for the pixel extract
			int pixShift	= 12;				// shift pixel to low 2 bits
							
			for (int i=0; i<pixPerWord; i++)
				{
				int pixel	= (val & pixMask) >> pixShift;
				pixMask   >>= 4;
				pixShift   -= 4;
				
				p[0] 		= (p[0] << 1) | (pixel >> 3);
				p[1]		= (p[1] << 1) | ((pixel >> 2) & 1);
				p[2]		= (p[2] << 1) | ((pixel >> 1) & 1);
				p[3]		= (p[3] << 1) | (pixel & 1);;
				}
			
			/*****************************************************************\
			|* If this is an 'end' pixel, then store the aggregated values
			|* then clear them
			\*****************************************************************/
			if (((j&3) == 3))
				{
				bitsToProcess = 0;
				for (int i=0; i<4; i++)
					*ptr[i] ++ = p[i];
				p[0] = p[1] = p[2] = p[3] = 0;
				}
			}
		if (bitsToProcess)
			{
			for (int i=0; i<4; i++)
				*ptr[i] ++ = p[i];
			p[0] = p[1] = p[2] = p[3] = 0;
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		uint8_t * srcRgb = data;
		uint8_t * dstRgb = (uint8_t *)ptr[3];
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
	
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_STANDARD;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerPlanePerLine;
	dst->fd_nplanes	= 4;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}
	
	
/*****************************************************************************\
|* Convert 8-bit indexed to planar
\*****************************************************************************/
static void _convToPlanar8(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	
	/*************************************************************************\
	|* Each plane in dst is 16-bits in quantum
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);

	/*************************************************************************\
	|* Multiply by 8 planes and H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired		 	= wordsPerPlanePerLine * 8 * H * 2;

	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
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
	|* Run over the source data, plane-ifying
	\*************************************************************************/
	uint8_t *data = src->fd_addr;
	uint16_t p[8] = {0,0,0,0,0,0,0,0};			// Aggregated plane values
	for (int i=0; i<H; i++)
		{
		/*********************************************************************\
		|* For each line of words...
		\*********************************************************************/
		int words = src->fd_wdwidth;
		
		int bitsToProcess 	= 1;
		int shift 			= 16;
		
		for (int j=0; j<words; j++)
			{
			bitsToProcess = 1;
			
			/*****************************************************************\
			|* Read the next word and decompose into pixel values
			\*****************************************************************/
			uint16_t val 	= *data ++;
			val				= (val << 8) | *data ++;
			
			/*****************************************************************\
			|* For each pixel, disperse over the 8 planes
			\*****************************************************************/
			int pixPerWord 	= 2;				// 2 pixels per 16-bit word
			int pixMask		= 0xFF00;			// Mask for the pixel extract
			int pixShift	= 8;				// shift pixel to low 2 bits
							
			for (int i=0; i<pixPerWord; i++)
				{
				int pixel	= (val & pixMask) >> pixShift;
				pixMask   >>= 8;
				pixShift   -= 8;
				
				p[0] 		= (p[0] << 1) | (pixel >> 7);
				p[1]		= (p[1] << 1) | ((pixel >> 6) & 1);
				p[2]		= (p[2] << 1) | ((pixel >> 5) & 1);
				p[3]		= (p[3] << 1) | ((pixel >> 4) & 1);
				p[4]		= (p[4] << 1) | ((pixel >> 3) & 1);
				p[5]		= (p[5] << 1) | ((pixel >> 2) & 1);
				p[6]		= (p[6] << 1) | ((pixel >> 1) & 1);
				p[7]		= (p[7] << 1) | (pixel & 1);;
				
				shift --;
				}
			
			/*****************************************************************\
			|* If this is an 'end' pixel, then store the aggregated values
			|* then clear them
			\*****************************************************************/
			if (((j&7) == 7))
				{
				bitsToProcess = 0;
				for (int i=0; i<8; i++)
					*ptr[i] ++ = p[i];
				memset(p, 0, 8*2);
				}
			}
		if (bitsToProcess)
			{
			for (int i=0; i<8; i++)
				*ptr[i] ++ = p[i] << shift;
				memset(p, 0, 8*2);
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		uint8_t * srcRgb = data;
		uint8_t * dstRgb = (uint8_t *)ptr[7];
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
	
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
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
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 16-bit RGB to planar
\*****************************************************************************/
static void _convToPlanar16(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	/*************************************************************************\
	|* Each plane in dst is 16-bits in quantum
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
static void _convToPlanar24(MFDB *src, MFDB *dst)
	{
	int W = src->fd_w;
	int H = src->fd_h;
	
	/*************************************************************************\
	|* Each plane in dst is 16-bits in quantum
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



#pragma mark - planar -> chunky

/*****************************************************************************\
|* Convert 2-bit planar to chunky
\*****************************************************************************/
static void _convToChunky2(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	int words			= src->fd_wdwidth;
	
	/*************************************************************************\
	|* Figure out how many words we need per line. Each pixel takes 4 bits
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);
	int wordsPerLine			= (W/8)  + (((W &  7) != 0) ? 1 : 0);
	
	/*************************************************************************\
	|* Take into account the number of planes
	\*************************************************************************/
	wordsPerLine *= src->fd_nplanes;
	
	/*************************************************************************\
	|* Multiply by H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired			= wordsPerLine * H * 2;
	
	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the each plane's data
	\*************************************************************************/
	uint16_t *ptr[2];
	for (int i=0; i<2; i++)
		ptr[i] = (uint16_t *)(src->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
	/*************************************************************************\
	|* Run over the source data, chunk-ifying
	\*************************************************************************/
	uint8_t *d[2];								// Data pointers to planes
	uint8_t *pix = NULL;
	
	for (int i=0; i<H; i++)
		{
		d[0]	= ((uint8_t *) ptr[0]) 	  	+ (i * words 		* 2);
		d[1]	= ((uint8_t *) ptr[1]) 	  	+ (i * words 		* 2);
		pix 	= ((uint8_t *)dst->fd_addr)	+ (i * wordsPerLine * 2);
		
		for (int j=0; j<words; j++)
			{
			uint16_t p[2] = {0,0};				// Aggregated plane values
			/*****************************************************************\
			|* Read the next word from each plane
			\*****************************************************************/
			for (int k=0; k<2; k++)
				{
				p[k] 	= (*d[k] ++) << 8;
				p[k]   |= *d[k] ++;
				}

			/*****************************************************************\
			|* Compose pixels from the plane data and store them
			\*****************************************************************/
			int mask	= 0x8000;
			uint8_t val	= 0;
			
			for (int k=0; k<16; k++)
				{
				static int map[2] = {2, 1};
				
				val <<= 2;
//				val += (p[0] & mask) == 0 ? 0 : 2;
//				val += (p[1] & mask) == 0 ? 0 : 1;
				
				for (int l=0; l<2; l++)
					val |= (p[l] & mask) != 0 ? map[l] : 0;
				mask >>= 1;
				
				if ((k&3) == 3)
					{
					*pix++ = val;
					val = 0;
					}
				}
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		int srcOffset 		= H * words * 2 /* -> bytes */ *2; /* planes */
		uint8_t * srcRgb 	= ((uint8_t *)src->fd_addr) + srcOffset;
		uint8_t * dstRgb 	= ((uint8_t *)dst->fd_addr) + (H * wordsPerLine * 2);
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_DEVICE;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerLine;
	dst->fd_nplanes	= 2;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 4-bit planar to chunky
\*****************************************************************************/
static void _convToChunky4(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	int words			= src->fd_wdwidth;
	
	/*************************************************************************\
	|* Figure out how many words we need per line. Each pixel takes 2 bits
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);
	int wordsPerLine			= (W/4)  + (((W &  3) != 0) ? 1 : 0);
		
	/*************************************************************************\
	|* Multiply by H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired			= wordsPerLine * H * 2;
	
	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the each plane's data
	\*************************************************************************/
	uint16_t *ptr[4];
	for (int i=0; i<4; i++)
		{
		ptr[i] = (uint16_t *)(src->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
	
		//printf("ptr[%d] set to %p (+%d)\n", i, ptr[i], ptr[i] - (uint16_t*)src->fd_addr);
		}
		
	/*************************************************************************\
	|* Run over the source data, chunk-ifying
	\*************************************************************************/
	uint8_t *d[4];								// Data pointers to planes
	uint8_t *pix = NULL;
	
	for (int i=0; i<H; i++)
		{
		d[0]	= ((uint8_t *) ptr[0]) 	  	+ (i * words 		* 2);
		d[1]	= ((uint8_t *) ptr[1]) 	  	+ (i * words 		* 2);
		d[2]	= ((uint8_t *) ptr[2]) 	  	+ (i * words 		* 2);
		d[3]	= ((uint8_t *) ptr[3]) 	  	+ (i * words 		* 2);
		pix 	= ((uint8_t *)dst->fd_addr)	+ (i * wordsPerLine * 2);
		
		for (int j=0; j<W;)
			{
			uint16_t p[4] = {0,0,0,0};			// Aggregated plane values
			/*****************************************************************\
			|* Read the next word from each plane
			\*****************************************************************/
			for (int k=0; k<4; k++)
				{
				uint16_t lo = *d[k] ++;
				uint16_t hi	= *d[k] ++;
				p[k]   		= (hi<<8) | lo;
				//printf("loading %04x into plane %d\n", p[k], k);
				}

			/*****************************************************************\
			|* Compose pixels from the plane data and store them
			\*****************************************************************/
			int mask	= 0x8000;
			uint8_t val	= 0;
			int written	= 0;
			int loops	= MIN(W-j, 16);
			
			for (int k=0; k<loops; k++)
				{
				written = 0;
				static int map[4] = {8, 4, 2, 1};
				
				val <<= 4;
				for (int l=0; l<4; l++)
					val |= (p[l] & mask) != 0 ? map[l] : 0;
				mask >>= 1;
				
				if ((k&1) == 1)
					{
					written = 1;
					//fprintf(stderr, "  writing %02x to %p\n", val, pix);
					*pix++ = val;
					val = 0;
					}
				j++;
				}
			if (written == 0)
				{
				written = 1;
				//fprintf(stderr, "+ writing %02x to %p\n", val, pix);
				*pix++ = val;
				val = 0;
				}
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		int srcOffset 		= H * words * 2 /* -> bytes */ * 4; /* planes */
		uint8_t * srcRgb 	= ((uint8_t *)src->fd_addr) + srcOffset;
		uint8_t * dstRgb 	= ((uint8_t *)dst->fd_addr) + (H * wordsPerLine * 2);
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_DEVICE;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerLine;
	dst->fd_nplanes	= 4;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 8-bit planar to chunky
\*****************************************************************************/
static void _convToChunky8(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	int words			= src->fd_wdwidth;
	
	/*************************************************************************\
	|* Figure out how many words we need per line. Each pixel takes 2 bits
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);
	int wordsPerLine			= (W/2)  + (W & 1);
		
	/*************************************************************************\
	|* Multiply by H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired			= wordsPerLine * H * 2;
	
	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the each plane's data
	\*************************************************************************/
	uint16_t *ptr[8];
	for (int i=0; i<8; i++)
		ptr[i] = (uint16_t *)(src->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
		
	/*************************************************************************\
	|* Run over the source data, chunk-ifying
	\*************************************************************************/
	uint8_t *d[8];								// Data pointers to planes
	uint8_t *pix = NULL;
	
	for (int i=0; i<H; i++)
		{
		for (int j=0; j<8; j++)
			d[j]	= ((uint8_t *) ptr[j]) 	  	+ (i * words 		* 2);
		pix 		= ((uint8_t *)dst->fd_addr)	+ (i * wordsPerLine * 2);
		
		for (int j=0; j<W;)
			{
			uint16_t p[8];						// Aggregated plane values
			memset(p, 0, 16);
			
			/*****************************************************************\
			|* Read the next word from each plane
			\*****************************************************************/
			for (int k=0; k<8; k++)
				{
				p[k] 	= (*d[k] ++) << 8;
				p[k]   |= *d[k] ++;
				}

			/*****************************************************************\
			|* Compose pixels from the plane data and store them
			\*****************************************************************/
			int mask	= 0x8000;
			int written	= 0;
			int loops	= MIN(W-j, 16);
			
			for (int k=0; k<loops; k++)
				{
				written = 0;
				static int map[8] = {128, 64, 32, 16, 8, 4, 2, 1};
				
				uint8_t val = 0;
				for (int l=0; l<8; l++)
					val |= (p[l] & mask) != 0 ? map[l] : 0;
				mask >>= 1;
				
				*pix++ = val;
				j++;
				}
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		int srcOffset 		= H * words * 2 /* -> bytes */ * 8; /* planes */
		uint8_t * srcRgb 	= ((uint8_t *)src->fd_addr) + srcOffset;
		uint8_t * dstRgb 	= ((uint8_t *)dst->fd_addr) + (H * wordsPerLine * 2);
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_DEVICE;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerLine;
	dst->fd_nplanes	= 8;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 16-bit planar to chunky
\*****************************************************************************/
static void _convToChunky16(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	int words			= src->fd_wdwidth;
	
	/*************************************************************************\
	|* Figure out how many words we need per line. Each pixel takes 2 bits
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16) + (((W & 15) != 0) ? 1 : 0);
	int wordsPerLine			= W;
		
	/*************************************************************************\
	|* Multiply by H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired			= wordsPerLine * H * 2;
	
	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the each plane's data
	\*************************************************************************/
	uint16_t *ptr[16];
	for (int i=0; i<16; i++)
		{
		ptr[i] = (uint16_t *)(src->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
		}
		
	/*************************************************************************\
	|* Run over the source data, chunk-ifying
	\*************************************************************************/
	uint8_t *d[16];								// Data pointers to planes
	uint8_t *pix = NULL;
	
	for (int i=0; i<H; i++)
		{
		for (int j=0; j<16; j++)
			d[j]	= ((uint8_t *) ptr[j]) 	  	+ (i * words 		* 2);
		pix 		= ((uint8_t *)dst->fd_addr)	+ (i * wordsPerLine * 2);
		
		for (int j=0; j<W;)
			{
			uint16_t p[16];						// Aggregated plane values
			memset(p, 0, 32);
			
			/*****************************************************************\
			|* Read the next word from each plane
			\*****************************************************************/
			for (int k=0; k<16; k++)
				{
				p[k] 	= (*d[k] ++) << 8;
				p[k]   |= *d[k] ++;
				}

			/*****************************************************************\
			|* Compose pixels from the plane data and store them
			\*****************************************************************/
			int mask	= 0x8000;
			int written	= 0;
			int loops	= MIN(W-j, 16);
			
			for (int k=0; k<loops; k++)
				{
				written = 0;
				static int map[16] = {32768, 16384, 8192, 4096,
									  2048, 1024, 512, 256,
									  128, 64, 32, 16,
									  8, 4, 2, 1};
				
				uint16_t val = 0;
				for (int l=0; l<16; l++)
					val |= (p[l] & mask) != 0 ? map[l] : 0;
				mask >>= 1;
								
				*pix++ = val >> 8;
				*pix++ = val & 0xff;
				j++;
				}
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		int srcOffset 		= H * words * 2 /* -> bytes */ * 16; /* planes */
		uint8_t * srcRgb 	= ((uint8_t *)src->fd_addr) + srcOffset;
		uint8_t * dstRgb 	= ((uint8_t *)dst->fd_addr) + (H * wordsPerLine * 2);
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_DEVICE;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerLine;
	dst->fd_nplanes	= 16;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

/*****************************************************************************\
|* Convert 24-bit planar to chunky
\*****************************************************************************/
static void _convToChunky24(MFDB *src, MFDB *dst)
	{
	int W 				= src->fd_w;
	int H 				= src->fd_h;
	int colourMapSize 	= src->fd_r3;
	int words			= src->fd_wdwidth;
	
	/*************************************************************************\
	|* Figure out how many words we need per line. Each pixel takes 2 bits
	\*************************************************************************/
	int wordsPerPlanePerLine	= (W/16)  + (((W & 15) != 0) ? 1 : 0);
	int wordsPerLine			= (W*3)/2 + (W&1);
		
	/*************************************************************************\
	|* Multiply by H lines, then x2 to get the bytes required
	\*************************************************************************/
	int bytesRequired			= wordsPerLine * H * 2;
	
	/*************************************************************************\
	|* Add on space for the RGB values for each of the 4 possible colours
	\*************************************************************************/
	if (colourMapSize)
		bytesRequired 			+= colourMapSize * 3;
	
	/*************************************************************************\
	|* Handle memory allocation
	\*************************************************************************/
	int needsCopy = _allocMFDB(src, dst, bytesRequired);
	if (needsCopy < 0)
		return;
		
	/*************************************************************************\
	|* Set up pointers to the start of the each plane's data
	\*************************************************************************/
	uint16_t *ptr[24];
	for (int i=0; i<24; i++)
		{
		ptr[i] = (uint16_t *)(src->fd_addr)
			   + i * wordsPerPlanePerLine * src->fd_h;
		}
		
	/*************************************************************************\
	|* Run over the source data, chunk-ifying
	\*************************************************************************/
	uint8_t *d[24];								// Data pointers to planes
	uint8_t *pix = NULL;
	
	for (int i=0; i<H; i++)
		{
		for (int j=0; j<24; j++)
			d[j]	= ((uint8_t *) ptr[j]) 	  	+ (i * words 		* 2);
		pix 		= ((uint8_t *)dst->fd_addr)	+ (i * wordsPerLine * 2);
		
		for (int j=0; j<W;)
			{
			uint16_t p[24];						// Aggregated plane values
			memset(p, 0, 48);
			
			/*****************************************************************\
			|* Read the next word from each plane
			\*****************************************************************/
			for (int k=0; k<24; k++)
				{
				p[k] 	= (*d[k] ++) << 8;
				p[k]   |= *d[k] ++;
				}

			/*****************************************************************\
			|* Compose pixels from the plane data and store them
			\*****************************************************************/
			int mask	= 0x8000;
			int written	= 0;
			int loops	= MIN(W-j, 24);
			
			for (int k=0; k<loops; k++)
				{
				written = 0;
				static int map[24] = {0x800000, 0x400000, 0x200000, 0x100000,
									  0x080000, 0x040000, 0x020000, 0x010000,
									  0x008000, 0x004000, 0x002000, 0x001000,
									  0x000800, 0x000400, 0x000200, 0x000100,
									  0x000080, 0x000040, 0x000020, 0x000010,
									  0x000008, 0x000004, 0x000002, 0x000001};
				
				uint32_t val = 0;
				for (int l=0; l<24; l++)
					val |= (p[l] & mask) != 0 ? map[l] : 0;
				mask >>= 1;
								
				*pix++ = val >> 16;
				*pix++ = (val >> 8) & 0xff;
				*pix++ = val & 0xff;
				j++;
				}
			}
		}
		
	/*************************************************************************\
	|* We have the RGB values at the end of the source pixel data
	\*************************************************************************/
	if (colourMapSize)
		{
		int srcOffset 		= H * words * 2 /* -> bytes */ * 16; /* planes */
		uint8_t * srcRgb 	= ((uint8_t *)src->fd_addr) + srcOffset;
		uint8_t * dstRgb 	= ((uint8_t *)dst->fd_addr) + (H * wordsPerLine * 2);
		memcpy(dstRgb, srcRgb, colourMapSize * 3);
		}
		
	/*************************************************************************\
	|* If MFDB pointers were the same, copy the results back in and free
	\*************************************************************************/
	if (needsCopy)
		{
		memcpy(src->fd_addr, dst->fd_addr, bytesRequired);
		free(dst->fd_addr);
		dst->fd_addr = src->fd_addr;
		}
	
	dst->fd_stand 	= MFDB_DEVICE;
	dst->fd_w		= W;
	dst->fd_h		= H;
	dst->fd_wdwidth	= wordsPerLine;
	dst->fd_nplanes	= 16;
	dst->fd_r1		= 0;
	dst->fd_r2		= 0;
	dst->fd_r3		= colourMapSize;
	}

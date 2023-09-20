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
void v_trnfm(int16_t handle, MFDB *src, MFDB *dst)
	{
	/*************************************************************************\
	|* Check we don't have NULL pointers
	\*************************************************************************/
	if ((src == NULL) || (dst == NULL))
		{
		WARN("src/dst passed as NULL");
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
				//_convPlanar16(src, dst);
				break;
			case 24:
				//_convPlanar24(src, dst);
				break;
			default:
				fprintf(stderr, "Unknown sourcce bitmap format %d in v_trnfm\n",
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
			//_convPlanarGreyscale8(src, dst);
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
			WARN("Unsupported bit depth %d in v_trnfm\n", src->fd_r3);
			break;
		}
	}

/*****************************************************************************\
|* Convert 8-bit greyscale to planar
\*****************************************************************************/
static void _convPlanarGreyscale8(MFDB *src, MFDB *dst)
	{
	if (dst->fd_addr == NULL)
		{
		dst->fd_addr = malloc(src->fd_w * src->fd_h);
		}
	}

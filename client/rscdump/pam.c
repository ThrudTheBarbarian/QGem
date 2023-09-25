//
//  pam.c
//  rscdump
//
//  Created by Simon Gornall on 9/24/23.
//

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "pam.h"

/*****************************************************************************\
|* Typedefs and globals
\*****************************************************************************/
typedef struct
	{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	} RGB;

typedef struct
	{
	RGB rgb;
	uint8_t a;
	} RGBA;
	
static RGB _palette16[16] = {
	{255, 255, 255},		// white
	{128, 128, 128}, 		// dark grey
	{0, 0, 255},			// blue
	{0, 0, 182},			// dark blue
	{0, 255, 0},			// green
	{0, 182, 0},			// dark green
	{0, 255, 255},			// cyan
	{0, 182, 182},			// dark cyan
	{255, 0, 0},			// red
	{182, 0, 0},			// dark red
	{255, 0, 255},			// magenta
	{182, 0, 182},			// dark magenta
	{255, 255, 0},			// yellow
	{182, 182, 0},			// dark yellow
	{192, 192, 192},		// light grey
	{0, 0, 0},				// black
	};

static char _pix[16] = {'.','X','R','G','B','C','Y','M',
					    'w','x','r','g','b','c','y','m'};

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
int _writePam16(FILE *fp, MFDB *img, MFDB *mask);

/*****************************************************************************\
|* Write a PAM file from image data, with an optional mask
\*****************************************************************************/
int writePAM(const char *stem, MFDB *imgData, MFDB *imgMask)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* Open the file for writing
	\*************************************************************************/
	char filename[PATH_MAX];
	sprintf(filename, "%s.pam", stem);
	FILE *fp = fopen(filename, "wb");
	if (fp != NULL)
		{
		/*********************************************************************\
		|* Choose which depth of file to write
		\*********************************************************************/
		switch (imgData->fd_nplanes)
			{
			case 4:
				ok = _writePam16(fp, imgData, imgMask);
				break;
			}
		}

	/*************************************************************************\
	|* Close the file
	\*************************************************************************/
	fclose(fp);
	
	return ok;
	}

/*****************************************************************************\
|* Write a 4-bit PAM file, with ot without an alpha channel
\*****************************************************************************/
int _writePam16(FILE *fp, MFDB *img, MFDB *mask)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* Decide on the format
	\*************************************************************************/
	int depth = 3;
	char *tuple = "RGB";
	if (mask->fd_addr != NULL)
		{
		depth = 4;
		tuple = "RGB_ALPHA";
		}
	
	/*************************************************************************\
	|* Output the header
	\*************************************************************************/
	fprintf(fp, "P7\n"
				"WIDTH %d\n"
				"HEIGHT %d\n"
				"DEPTH %d\n"
				"MAXVAL 255\n"
				"TUPLTYPE %s\n"
				"ENDHDR\n",
				img->fd_w,
				img->fd_h,
				depth,
				tuple);

	/*************************************************************************\
	|* Reserve enough temporary space for an RGBA image
	\*************************************************************************/
	RGBA *rgba = (RGBA *) malloc (img->fd_h * img->fd_w * sizeof(RGBA));
	if (rgba != NULL)
		{
		RGBA *rgbp = rgba;
		
		/*********************************************************************\
		|* Populate the RGB values
		\*********************************************************************/
		int words = img->fd_w/4;
		for (int i=0; i<img->fd_h; i++)
			{
			uint16_t *bits 		= ((uint16_t *)img->fd_addr)
								+ img->fd_wdwidth * i;
			for (int j=0; j<words; j++)
				{
				uint16_t bitmask  	= 0xF000;
				int shift			= 12;
				uint16_t data 		= ntohs(*bits ++);
				for (int k=0; k<4; k++)
					{
					uint8_t pixel = (data & bitmask) >> shift;
					bitmask >>= 4;
					shift -= 4;
					rgbp->rgb = _palette16[pixel];
					rgbp ++;
					}
				}
			}
			
		/*********************************************************************\
		|* If we have a mask, populate the alpha values
		\*********************************************************************/
		if (mask->fd_addr != NULL)
			{
			rgbp 	= rgba;
			words 	= img->fd_w/16;
			
			for (int i=0; i<img->fd_h; i++)
				{
				uint16_t *bits		= ((uint16_t *)mask->fd_addr)
									+ mask->fd_wdwidth * i;
				uint16_t bitmask  	= 0x8000;
				int shift			= 15;
				for (int j=0; j<words; j++)
					{
					uint16_t data = *bits ++;
					for (int k=0; k<16; k++)
						{
						uint8_t pixel = (data & bitmask) >> shift;
						bitmask >>= 1;
						shift -= 1;
						rgbp->a = pixel ? 255 : 0;
						rgbp ++;
						}
					}
				}
			}

		/*********************************************************************\
		|* Write out the file
		\*********************************************************************/
		int num = img->fd_h * img->fd_w;
		if (mask->fd_addr != NULL)
			fwrite(rgba, sizeof(RGBA), num, fp);
		else
			for (int i=0; i<num; i++)
				fwrite(&(rgba[i].rgb), sizeof(RGB), 1, fp);
				
		/*********************************************************************\
		|* Housekeeping
		\*********************************************************************/
		free(rgba);
		}
	return ok;
	}

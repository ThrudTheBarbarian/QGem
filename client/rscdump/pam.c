//
//  pam.c
//  rscdump
//
//  Created by Simon Gornall on 9/24/23.
//

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "palette.h"
#include "pam.h"

static RGB _palette16[16] = {
	{255, 255, 255},		// white
	{255, 0, 0},			// red
	{0, 255, 0},			// green
	{255, 255, 0},			// yellow
	{0, 0, 255},			// blue
	{255, 0, 255},			// magenta
	{0, 255, 255},			// cyan
	{192, 192, 192},		// light grey
	{128, 128, 128}, 		// dark grey
	{182, 0, 0},			// dark red
	{0, 182, 0},			// dark green
	{182, 182, 0},			// dark yellow
	{0, 0, 182},			// dark blue
	{182, 0, 182},			// dark magenta
	{0, 182, 182},			// dark cyan
	{0, 0, 0},				// black
	};


static const short devtovdi8[256] =
	{
	0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11,14,12,15,13,255,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,1
	};

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
int _writePam16(FILE *fp, MFDB *img, MFDB *mask);
int _writePam256(FILE *fp, MFDB *img, MFDB *mask);

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
			case 8:
				ok = _writePam256(fp, imgData, imgMask);
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
				for (int j=0; j<words; j++)
					{
					uint16_t bitmask  	= 0x8000;
					int shift			= 15;
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

/*****************************************************************************\
|* Write an 8-bit PAM file, with ot without an alpha channel
\*****************************************************************************/
int _writePam256(FILE *fp, MFDB *img, MFDB *mask)
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
		for (int i=0; i<img->fd_h; i++)
			{
			uint8_t *bits 		= ((uint8_t *)img->fd_addr)
								+ img->fd_wdwidth * i * 2;
			for (int j=0; j<img->fd_w; j++)
				{
				uint8_t pixel = *bits ++;
				rgbp->rgb = _palette[devtovdi8[pixel]];
				rgbp ++;
				}
			}
			
		/*********************************************************************\
		|* If we have a mask, populate the alpha values
		\*********************************************************************/
		int words = img->fd_w/2;
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
					uint16_t data = ntohs(*bits ++);
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

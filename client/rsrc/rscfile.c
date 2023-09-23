//
//  rscfile.c
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "rscfile.h"


/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static int _parseObjects(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseCIcons(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _readCiconBlock(FILE *fp, RscFileHeader *hdr, RscFile *rsc, int n);

/*****************************************************************************\
|* Read an RSC file into memory
\*****************************************************************************/
int resourceLoad(const char * filename, RscFile *rsc)
	{
	int ok = 0;
	
	FILE *fp = fopen(filename, "rb");
	if (fp != NULL)
		{
		RscFileHeader hdr;
		if (fread(&hdr, sizeof(RscFileHeader), 1, fp) == 1)
			{
			uint16_t *toSwap = (uint16_t *)&hdr;
			for (int i=0; i<18; i++)
				{
				*toSwap = ntohs(*toSwap);
				toSwap++;
				}
			rsc->version 	= hdr.rsh_vrsn;
			
			_parseObjects(fp, &hdr, rsc);
			_parseCIcons(fp, &hdr, rsc);
			}
		else
			perror("Reading RSC file header");
		}
	
	return ok;
	}

/*****************************************************************************\
|* Parse the objects out of the file
\*****************************************************************************/
static int _parseObjects(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nObjects	= hdr->rsh_nobs;
	return 1;
	}

/*****************************************************************************\
|* Parse the CICONs out of the file
\*****************************************************************************/
static int _parseCIcons(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	uint32_t cIconOffset = 0;
	/*************************************************************************\
	|* If we have the correct version
	\*************************************************************************/
	if (hdr->rsh_vrsn == 4)
		{
		fseek(fp, hdr->rsh_rssize, SEEK_SET);
		uint32_t token = (uint32_t)-1;
		int idx = 0;
		
		/*********************************************************************\
		|* Read extension pointers until done
		\*********************************************************************/
		while (token != 0)
			{
			fread(&token, sizeof(uint32_t), 1, fp);
			token = ntohl(token);
			switch (idx)
				{
				case 0:
					rsc->size = token;
					break;
				case 1:
					cIconOffset = token;
					break;
				}
			idx ++;
			}
		
		/*********************************************************************\
		|* If we have the CICONBLK extension pointer, then parse it
		\*********************************************************************/
		if (cIconOffset)
			{
			fseek(fp, cIconOffset, SEEK_SET);
			rsc->nCicons = 0;
			while (token != (uint32_t)-1)
				{
				fread(&token, sizeof(uint32_t), 1, fp);
				token = ntohl(token);
				if (token == 0)
					rsc->nCicons ++;
				}
			rsc->cIcons = (CICONBLK*)malloc(sizeof(CICONBLK) * rsc->nCicons);
			if (rsc->cIcons == NULL)
				return 0;
			
			for (int i=0; i<rsc->nCicons; i++)
				_readCiconBlock(fp, hdr, rsc, i);
			}
		}
	else
		{
		fprintf(stderr, "RSC file version is only %d\n", hdr->rsh_vrsn);
		}
	return 1;
	}

/*****************************************************************************\
|* Helper functions to parse values
\*****************************************************************************/
static inline int _fetch32(void *ptr, int offset, uint32_t* value)
	{
	uint8_t *bPtr 	= ((uint8_t *)ptr) + offset;
	uint32_t val	= *((uint32_t *)bPtr);
	*value 			= ntohl(val);
	return offset + 4;
	}
	
static inline int _fetch16(void *ptr, int offset, uint16_t* value)
	{
	uint8_t *bPtr 	= ((uint8_t *)ptr) + offset;
	uint16_t val	= *((uint16_t *)bPtr);
	*value 			= ntohs(val);
	return offset + 2;
	}
	
/*****************************************************************************\
|* Read in the CICONBLK structures and parse
\*****************************************************************************/
static int _readCiconBlock(FILE *fp, RscFileHeader *hdr, RscFile *rsc, int idx)
	{
	/*************************************************************************\
	|* Where it goes
	\*************************************************************************/
	ICONBLK *iblk = &(rsc->cIcons[idx].monoIcon);
	
	/*************************************************************************\
	|* Read in the data for the monochrome icon structure
	\*************************************************************************/
	fseek(fp, 12, SEEK_CUR);		// Skip over the zeros in the file
	uint8_t data[12*2];				// Storage on disk isn't the same as RAM
	fread(data, 24, 1, fp);
	int cursor  = 0;
	
	cursor		= _fetch16(data, cursor, &(iblk->ib_char));
	cursor		= _fetch16(data, cursor, &(iblk->ib_xchar));
	cursor		= _fetch16(data, cursor, &(iblk->ib_ychar));
	cursor		= _fetch16(data, cursor, &(iblk->ib_xicon));
	cursor		= _fetch16(data, cursor, &(iblk->ib_yicon));
	cursor		= _fetch16(data, cursor, &(iblk->ib_wicon));
	cursor		= _fetch16(data, cursor, &(iblk->ib_hicon));
	cursor		= _fetch16(data, cursor, (uint16_t *)(&(iblk->ib_xtext)));
	cursor		= _fetch16(data, cursor, &(iblk->ib_ytext));
	cursor		= _fetch16(data, cursor, &(iblk->ib_wtext));
	cursor		= _fetch16(data, cursor, &(iblk->ib_htext));
	cursor		= _fetch16(data, cursor, &(iblk->ib_resvd));

	/*************************************************************************\
	|* Read in the number of colour icons, one per resolution
	\*************************************************************************/
	int16_t ncIcons = 0;
	if (fread(&ncIcons, 2, 1, fp) != 1)
		{
		WARN("Cannot read number of colour icons for icon %d", idx);
		return 0;
		}
	ncIcons = ntohs(ncIcons);
	
	/*************************************************************************\
	|* Read in the data for the monochrome data
	\*************************************************************************/
	int iconW	= iblk->ib_wicon / 16
				+ (((iblk->ib_wicon & 15) != 0) ? 1 : 0);
	int words	= iblk->ib_hicon * iconW;
	
	iblk->ib_pdata	= (uint16_t *) malloc (words * 2);
	if (iblk->ib_pdata == NULL)
		{
		WARN("Cannot allocate mono data for icon %d", idx);
		return 0;
		}
		
	if (fread(iblk->ib_pdata, words, 2, fp) != 2)
		{
		WARN("Cannot read mono data for icon %d", idx);
		return 0;
		}
		
	/*************************************************************************\
	|* Read in the mask for the monochrome data
	\*************************************************************************/
	iblk->ib_pmask	= (uint16_t *) malloc (words * 2);
	if (iblk->ib_pmask == NULL)
		{
		WARN("Cannot allocate mono mask data for icon %d", idx);
		return 0;
		}
	if (fread(iblk->ib_pmask, words, 2, fp) != 2)
		{
		WARN("Cannot read mono mask data for icon %d", idx);
		return 0;
		}
		
	/*************************************************************************\
	|* Read in the icon string
	\*************************************************************************/
	iblk->ib_ptext	= (uint8_t *) malloc (13);
	if (iblk->ib_ptext == NULL)
		{
		WARN("Cannot allocate text data for icon %d", idx);
		return 0;
		}
	if (fread(iblk->ib_ptext, 12, 1, fp) != 1)
		{
		WARN("Cannot read text data for icon %d", idx);
		return 0;
		}
	iblk->ib_ptext[12] = '\0';
	
	printf("icon: '%s'\n", iblk->ib_ptext);
	return 1;
	}

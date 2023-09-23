//
//  rscfile.c
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#include <stdlib.h>

#include "rscfile.h"


/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static int _parseObjects(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseCIcons(FILE *fp, RscFileHeader *hdr, RscFile *rsc);

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
			rsc->cIcons = (CICONBLK**)malloc(sizeof(CICONBLK*) * rsc->nCicons);
			
			}
		}
	else
		{
		fprintf(stderr, "RSC file version is only %d\n", hdr->rsh_vrsn);
		}
	return 1;
	}

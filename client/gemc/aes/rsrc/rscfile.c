//
//  rscfile.c
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "rscfile.h"
#include "vec.h"

#ifndef SYSTEM_DIR
#  define SYSTEM_DIR "/usr/local/atari/System"
#endif

#ifndef ICON_DIR
#  define ICON_DIR "Icons"
#endif

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
static int _parseObjects(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseCIcons(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseTedInfo(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseIconBlks(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseBitBlks(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseFreeStrings(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _parseFreeImages(FILE *fp, RscFileHeader *hdr, RscFile *rsc);
static int _readCiconBlock(FILE *fp, RscFileHeader *hdr, RscFile *rsc, int n);


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
|* Read an RSC file into memory
\*****************************************************************************/
int resourceLoad(const char * filename, RscFile *rsc)
	{
	int ok = 0;
	
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
		{
		char path[PATH_MAX];
		snprintf(path, PATH_MAX, "%s/%s/%s", SYSTEM_DIR, ICON_DIR, filename);
		fp = fopen(path, "rb");
		}

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
			
			ok = _parseCIcons(fp, &hdr, rsc);
			ok = ok && _parseObjects(fp, &hdr, rsc);
			ok = ok && _parseTedInfo(fp, &hdr, rsc);
			ok = ok && _parseIconBlks(fp, &hdr, rsc);
			ok = ok && _parseBitBlks(fp, &hdr, rsc);
			ok = ok && _parseFreeStrings(fp, &hdr, rsc);
			ok = ok && _parseFreeImages(fp, &hdr, rsc);
			}
		else
			perror("Reading RSC file header");
		}
	
	return ok;
	}


/*****************************************************************************\
|* Parse the TEDINFO's out of the file
\*****************************************************************************/
static int _parseTedInfo(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nTedInfo	= hdr->rsh_nted;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_tedinfo, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the TEDINFO structures
	\*************************************************************************/
	rsc->tedInfo = (TEDINFO *) malloc(sizeof(TEDINFO) * rsc->nTedInfo);
	if (rsc->tedInfo == NULL)
		{
		WARN("Cannot allocate space for TEDINFO structures");
		return 0;
		}
	
	
	/*************************************************************************\
	|* Convert between file and memory versions...
	\*************************************************************************/
	uint32_t te_ptext[rsc->nTedInfo],
			 te_ptmplt[rsc->nTedInfo],
			 te_pvalid[rsc->nTedInfo];
			
	/*************************************************************************\
	|* Read in the objects
	\*************************************************************************/
	for (int i=0; i<rsc->nTedInfo; i++)
		{
		/*********************************************************************\
		|* Read the disk format of the structure
		\*********************************************************************/
		uint8_t data[28];		// Size of a TEDINFO on disk
		if (fread(data, 28, 1, fp) != 1)
			{
			WARN("Failed to read OBJECT %d", i);
			return 0;
			}
			
		/*********************************************************************\
		|* Create an TEDINFO structure from the data
		\*********************************************************************/
		int cursor  	= 0;
		TEDINFO *obj	= &(rsc->tedInfo[i]);
		cursor		= _fetch32(data, cursor, &(te_ptext[i]));
		cursor		= _fetch32(data, cursor, &(te_ptmplt[i]));
		cursor		= _fetch32(data, cursor, &(te_pvalid[i]));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_font)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_fontid)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_just)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_color)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_fontsize)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_thickness)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_txtlen)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->te_tmplen)));
		}
	
	/*************************************************************************\
	|* Run through the list, finding the text values and putting them into the
	|* TEDINFO structure
	\*************************************************************************/
	for (int i=0; i<rsc->nTedInfo; i++)
		{
		TEDINFO *ti = &(rsc->tedInfo[i]);

		/*********************************************************************\
		|* Read the text
		\*********************************************************************/
		fseek(fp, te_ptext[i], SEEK_SET);
		int num		= 1+ti->te_txtlen;
		ti->te_ptext = (int8_t *) calloc(num, 1);
		if (ti->te_ptext == NULL)
			{
			WARN("Cannot allocate %d text bytes in TEDINFO #%d\n", num, i);
			break;
			}
		fread(ti->te_ptext, num, 1, fp);
		
		/*********************************************************************\
		|* Read the validation string
		\*********************************************************************/
		fseek(fp, te_pvalid[i], SEEK_SET);
		ti->te_pvalid = (int8_t *) calloc(num, 1);
		if (ti->te_pvalid == NULL)
			{
			WARN("Cannot allocate %d validation bytes in TEDINFO #%d\n", num, i);
			break;
			}
		fread(ti->te_pvalid, num, 1, fp);
			
		/*********************************************************************\
		|* Read the template
		\*********************************************************************/
		fseek(fp, te_ptmplt[i], SEEK_SET);
		num = 1+ti->te_tmplen;
		ti->te_ptmplt = (int8_t *) calloc(num, 1);
		if (ti->te_ptmplt == NULL)
			{
			WARN("Cannot allocate %d template bytes in TEDINFO #%d\n", num, i);
			break;
			}
		fread(ti->te_ptmplt, num, 1, fp);
		}
	return 1;
	}


/*****************************************************************************\
|* Read in a monochrome icon block header
\*****************************************************************************/
static int _readIconBlkHeader(FILE *fp, ICONBLK *iblk)
	{
	int ok = 0;
	
	/*************************************************************************\
	|* Read in the data for the monochrome icon header
	\*************************************************************************/
	uint8_t data[12*2];				// Storage on disk isn't the same as RAM
	if (fread(data, 12, 1, fp) == 1)
		{
		int cursor  = 0;
		cursor		= _fetch32(data, cursor, (uint32_t *)(&(iblk->ib_pmask)));
		cursor		= _fetch32(data, cursor, (uint32_t *)(&(iblk->ib_pdata)));
		cursor		= _fetch32(data, cursor, (uint32_t *)(&(iblk->ib_ptext)));
		ok ++;
		}
		
	if (fread(data, 24, 1, fp) == 1)
		{
		int cursor  = 0;
		cursor		= _fetch16(data, cursor, &(iblk->ib_char));
		cursor		= _fetch16(data, cursor, &(iblk->ib_xchar));
		cursor		= _fetch16(data, cursor, &(iblk->ib_ychar));
		cursor		= _fetch16(data, cursor, &(iblk->ib_xicon));
		cursor		= _fetch16(data, cursor, &(iblk->ib_yicon));
		cursor		= _fetch16(data, cursor, &(iblk->ib_wicon));
		cursor		= _fetch16(data, cursor, &(iblk->ib_hicon));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(iblk->ib_xtext)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(iblk->ib_ytext)));
		cursor		= _fetch16(data, cursor, &(iblk->ib_wtext));
		cursor		= _fetch16(data, cursor, &(iblk->ib_htext));
		cursor		= _fetch16(data, cursor, &(iblk->ib_resvd));
		ok ++;
		}
		
	return (ok == 2) ? 1 : 0;
	}

/*****************************************************************************\
|* Read in a monochrome icon block data
\*****************************************************************************/
static int _readIconBlkData(FILE *fp, ICONBLK *iblk)
	{
	/*************************************************************************\
	|* Read in the monochrome icon data
	\*************************************************************************/
	int iconW	= iblk->ib_wicon / 16
				+ (((iblk->ib_wicon & 15) != 0) ? 1 : 0);
	int words	= iblk->ib_hicon * iconW;
	
	long offset	= (ptrdiff_t) iblk->ib_pdata;
	fseek(fp, offset, SEEK_SET);
	
	iblk->ib_pdata	= (uint16_t *) malloc (words * 2);
	if (iblk->ib_pdata == NULL)
		{
		WARN("Cannot allocate mono data for icon");
		return 0;
		}
		
	if (fread(iblk->ib_pdata, words, 2, fp) != 2)
		{
		WARN("Cannot read mono data for icon");
		return 0;
		}
	else
		{
		uint16_t *ptr = iblk->ib_pdata;
		for (int i=0; i<words; i++)
			{
			*ptr = ntohs(*ptr);
			ptr ++;
			}
		}
		
	/*************************************************************************\
	|* Read in the mask for the monochrome data
	\*************************************************************************/
	offset	= (ptrdiff_t) iblk->ib_pmask;
	fseek(fp, offset, SEEK_SET);
	
	iblk->ib_pmask	= (uint16_t *) malloc (words * 2);
	if (iblk->ib_pmask == NULL)
		{
		WARN("Cannot allocate mono mask data for icon");
		return 0;
		}
	if (fread(iblk->ib_pmask, words, 2, fp) != 2)
		{
		WARN("Cannot read mono mask data for icon");
		return 0;
		}
	else
		{
		uint16_t *ptr = iblk->ib_pmask;
		for (int i=0; i<words; i++)
			{
			*ptr = ntohs(*ptr);
			ptr ++;
			}
		}
		
	/*************************************************************************\
	|* Read in the icon string
	\*************************************************************************/
	offset	= (ptrdiff_t) iblk->ib_ptext;
	fseek(fp, offset, SEEK_SET);

	iblk->ib_ptext	= (int8_t *) malloc (13);
	if (iblk->ib_ptext == NULL)
		{
		WARN("Cannot allocate text data for icon");
		return 0;
		}
	if (fread(iblk->ib_ptext, 12, 1, fp) != 1)
		{
		WARN("Cannot read text data for icon");
		return 0;
		}
	iblk->ib_ptext[12] = '\0';
		
	return 1;
	}


/*****************************************************************************\
|* Parse the ICONBLK's out of the file
\*****************************************************************************/
static int _parseIconBlks(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nIconBlks	= hdr->rsh_nib;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_iconblk, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the ICONBLK structures
	\*************************************************************************/
	rsc->iconBlks = (ICONBLK *) malloc(sizeof(ICONBLK) * rsc->nIconBlks);
	if (rsc->iconBlks == NULL)
		{
		WARN("Cannot allocate space for %d ICONBLK structures", rsc->nIconBlks);
		return 0;
		}
				
	/*************************************************************************\
	|* Read in the objects
	\*************************************************************************/
	int ok = (rsc->nIconBlks == 0) ? 1 : 0;
	for (int i=0; i<rsc->nIconBlks; i++)
		{
		long offset = hdr->rsh_iconblk + i * 36;
		fseek(fp, offset, SEEK_SET);
		
		ICONBLK *blk = &(rsc->iconBlks[i]);
		
		ok = _readIconBlkHeader(fp, blk);
		if (ok == 0)
			break;
			
		ok = _readIconBlkData(fp, blk);
		if (ok == 0)
			break;
		}
	return ok;
	}

/*****************************************************************************\
|* Parse the free strings out of the file
\*****************************************************************************/
static int _parseFreeStrings(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nStrings	= hdr->rsh_nstring;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_frstr, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the strings
	\*************************************************************************/
	rsc->strings = (char **) malloc(sizeof(char*) * rsc->nStrings);
	if (rsc->strings == NULL)
		{
		WARN("Cannot allocate space for %d strings", rsc->nStrings);
		return 0;
		}
				
	/*************************************************************************\
	|* Read in the string pointers
	\*************************************************************************/
	uint8_t data[rsc->nStrings * 4];	// Size on disk not the same as RAM
	if (fread(data, 4, rsc->nStrings, fp) != rsc->nStrings)
		{
		WARN("Cannot read in the string offsets table");
		return 0;
		}
		
	/*************************************************************************\
	|* Read in the string data
	\*************************************************************************/
	int cursor	= 0;
	
	vec_char_t string;
	vec_init(&string);
	for (int i=0; i<rsc->nStrings; i++)
		{
		uint32_t offset;
		cursor	= _fetch32(data, cursor, &offset);
		fseek(fp, offset, SEEK_SET);
		
		vec_clear(&string);
		int c = fgetc(fp);
		while ((c != EOF) && (c != '\0'))
			{
			vec_push(&string, (char)c);
			c = fgetc(fp);
			}

		// trailing \0 is included
		rsc->strings[i] = (char *) malloc(string.length);
		if (rsc->strings[i] == NULL)
			{
			WARN("Cannot allocate space (%d) for string %d", string.length, i);
			return 0;
			}
			
		memcpy(rsc->strings[i], string.data, string.length);
		}
	vec_deinit(&string);
	
	return 1;
	}

/*****************************************************************************\
|* Read in a BITBLK at the current file offset
\*****************************************************************************/
static int _readBitBlk(FILE *fp, BITBLK *blk)
	{
	int ok = 0;
	
	uint8_t data[14];			// Storage on disk isn't the same as RAM
	if (fread(data, 14, 1, fp) == 1)
		{
		int cursor  = 0;
		cursor	= _fetch32(data, cursor, (uint32_t *)(&(blk->bi_pdata)));
		cursor	= _fetch16(data, cursor, (uint16_t *)(&(blk->bi_wb)));
		cursor	= _fetch16(data, cursor, (uint16_t *)(&(blk->bi_hl)));
		cursor	= _fetch16(data, cursor, (uint16_t *)(&(blk->bi_x)));
		cursor	= _fetch16(data, cursor, (uint16_t *)(&(blk->bi_y)));
		cursor	= _fetch16(data, cursor, (uint16_t *)(&(blk->bi_color)));

		long offset	= (ptrdiff_t) blk->bi_pdata;
		fseek(fp, offset, SEEK_SET);
		
		int bytes		= blk->bi_wb * blk->bi_hl;
		blk->bi_pdata	= (int16_t *) malloc (bytes);
		if (blk->bi_pdata == NULL)
			{
			WARN("Cannot allocate mono data for bitblk");
			return 0;
			}
			
		if (fread(blk->bi_pdata, bytes, 1, fp) != 1)
			{
			WARN("Cannot read mono data for bitblk");
			return 0;
			}
		else
			{
			int words = bytes / 2;
			uint16_t *ptr = (uint16_t *) blk->bi_pdata;
			for (int i=0; i<words; i++)
				{
				*ptr = ntohs(*ptr);
				ptr ++;
				}
			}
		ok = 1;
		}
	
	return ok;
	}
	
/*****************************************************************************\
|* Parse the BITBLK's out of the file
\*****************************************************************************/
static int _parseBitBlks(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nBitBlks	= hdr->rsh_nbb;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_bitblk, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the BITBLK structures
	\*************************************************************************/
	rsc->bitBlks = (BITBLK *) malloc(sizeof(BITBLK) * rsc->nBitBlks);
	if (rsc->bitBlks == NULL)
		{
		WARN("Cannot allocate space for %d BITBLK structures", rsc->nBitBlks);
		return 0;
		}
				
	/*************************************************************************\
	|* Read in the bitblks
	\*************************************************************************/
	for (int i=0; i<rsc->nBitBlks; i++)
		{
		long offset = hdr->rsh_bitblk + i * 14;
		fseek(fp, offset, SEEK_SET);
		
		BITBLK *blk = &(rsc->bitBlks[i]);
		if (_readBitBlk(fp, blk) == 0)
			{
			WARN ("Could not read BITBLK %d\n", i);
			return 0;
			}
		}
	return 1;
	}


/*****************************************************************************\
|* Parse the free images out of the file
\*****************************************************************************/
static int _parseFreeImages(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nImages	= hdr->rsh_nimages;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_frimg, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the strings
	\*************************************************************************/
	rsc->images = (BITBLK*) malloc(sizeof(BITBLK) * rsc->nImages);
	if (rsc->images == NULL)
		{
		WARN("Cannot allocate space for %d free images", rsc->nImages);
		return 0;
		}
				
	/*************************************************************************\
	|* Read in the image pointers
	\*************************************************************************/
	uint8_t data[rsc->nImages * 4];		// Size on disk not the same as RAM
	if (fread(data, 4, rsc->nImages, fp) != rsc->nImages)
		{
		WARN("Cannot read in the free image offsets table");
		return 0;
		}
		
	/*************************************************************************\
	|* Read in the image data
	\*************************************************************************/
	int cursor = 0;
	for (int i=0; i<rsc->nImages; i++)
		{
		BITBLK *blk = &(rsc->images[i]);

		uint32_t offset;
		cursor	= _fetch32(data, cursor, &offset);
		fseek(fp, offset, SEEK_SET);
		
		if (_readBitBlk(fp, blk) == 0)
			{
			WARN("Couldn't read free image %d", i);
			return 0;
			}
		}
	
	return 1;
	}


/*****************************************************************************\
|* Parse the objects out of the file
\*****************************************************************************/
static int _parseObjects(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	rsc->nObjects	= hdr->rsh_nobs;

	/*************************************************************************\
	|* Seek to the correct offset
	\*************************************************************************/
	fseek(fp, hdr->rsh_object, SEEK_SET);
	
	/*************************************************************************\
	|* Reserve space for all the OBJECT structures
	\*************************************************************************/
	rsc->objects = (OBJECT *) malloc(sizeof(OBJECT) * rsc->nObjects);
	if (rsc->objects == NULL)
		{
		WARN("Cannot allocate space for OBJECT structures");
		return 0;
		}
	
	/*************************************************************************\
	|* Read in the objects
	\*************************************************************************/
	for (int i=0; i<rsc->nObjects; i++)
		{
		/*********************************************************************\
		|* Read the disk format of the structure
		\*********************************************************************/
		uint8_t data[24];		// Size of an OBJECT on disk
		if (fread(data, 24, 1, fp) != 1)
			{
			WARN("Failed to read OBJECT %d", i);
			return 0;
			}
			
		/*********************************************************************\
		|* Create an OBJECT structure from the data
		\*********************************************************************/
		int cursor  = 0;
		OBJECT *obj	= &(rsc->objects[i]);
		uint32_t ob_spec;
		
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_next)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_head)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_tail)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_type)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_flags)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_state)));
		cursor		= _fetch32(data, cursor, &ob_spec);
		if ((ob_spec >= 0) && (ob_spec < rsc->nCicons))
			obj->ob_spec = (void *)(&(rsc->cIcons[ob_spec]));
			
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_x)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_y)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_width)));
		cursor		= _fetch16(data, cursor, (uint16_t *)(&(obj->ob_height)));
		}
	return 1;
	}

/*****************************************************************************\
|* Parse the CICONs out of the file
\*****************************************************************************/
static int _parseCIcons(FILE *fp, RscFileHeader *hdr, RscFile *rsc)
	{
	uint32_t cIconOffset 	= 0;
	uint32_t cPaletteOffset	= 0;
	
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
		for(;;)
			{
			fread(&token, sizeof(uint32_t), 1, fp);
			token = ntohl(token);
			if (token == 0)
				break;
			switch (idx)
				{
				case 0:
					rsc->size = token;
					break;
				case 1:
					cIconOffset = token;
					break;
				case 2:
					cPaletteOffset = token;
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
				{
				WARN("Cannot allocate CICONBLK memory for %d icons",
					 rsc->nCicons);
				return 0;
				}
			for (int i=0; i<rsc->nCicons; i++)
				_readCiconBlock(fp, hdr, rsc, i);
			}
		}

	return 1;
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
	cursor		= _fetch16(data, cursor, (uint16_t *)(&(iblk->ib_ytext)));
	cursor		= _fetch16(data, cursor, &(iblk->ib_wtext));
	cursor		= _fetch16(data, cursor, &(iblk->ib_htext));
	cursor		= _fetch16(data, cursor, &(iblk->ib_resvd));

	/*************************************************************************\
	|* Read in the number of colour icons, one per resolution
	\*************************************************************************/
	int16_t numIcons = 0;
	if (fread(&numIcons, 2, 1, fp) != 1)
		{
		WARN("Cannot read number of colour icons for icon %d", idx);
		return 0;
		}
	numIcons = ntohs(numIcons);
	
	/*************************************************************************\
	|* Read in the monochrome icon data
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
	else
		{
		uint16_t *ptr = iblk->ib_pdata;
		for (int i=0; i<words; i++)
			{
			*ptr = ntohs(*ptr);
			ptr ++;
			}
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
	else
		{
		uint16_t *ptr = iblk->ib_pmask;
		for (int i=0; i<words; i++)
			{
			*ptr = ntohs(*ptr);
			ptr ++;
			}
		}
		
	/*************************************************************************\
	|* Read in the icon string
	\*************************************************************************/
	iblk->ib_ptext	= (int8_t *) malloc (13);
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
		
	/*************************************************************************\
	|* Reserve the correct number of CICON structures
	\*************************************************************************/
	rsc->cIcons[idx].icons = (CICON *) malloc (sizeof(CICON) * numIcons);
	if (rsc->cIcons[idx].icons == NULL)
		{
		WARN("Cannot allocate space for %d colour icons for icon %d",
			 numIcons, idx);
		return 0;
		}
	
	for (int j=0; j<numIcons; j++)
		{
		rsc->cIcons[idx].icons[j].next = NULL;
		if (j>0)
			rsc->cIcons[idx].icons[j-1].next = &(rsc->cIcons[idx].icons[j]);
			
		/*********************************************************************\
		|* Read in the number of planes for this colour icon
		\*********************************************************************/
		int16_t planes;
		if (fread(&planes, 2, 1, fp) != 1)
			{
			WARN("Cannot read #planes for icon %d.%d", idx, j);
			return 0;
			}
		planes = ntohs(planes);
		rsc->cIcons[idx].icons[j].numPlanes = planes;

		/*********************************************************************\
		|* Read in the embedded pointers and figure out if there's 'sel' data
		\*********************************************************************/
		uint32_t ptrs[5];		// Embedded (usually nil) pointers in file
		if (fread(ptrs, 5*4, 1, fp) != 1)
			{
			WARN("Cannot read embedded ptrs for colour icon %d.%d", idx, j);
			return 0;
			}
		int selDataPresent 	= (ptrs[2] != 0) ? 1 : 0;
		
 		/*********************************************************************\
		|* Read in the data and form the structure: icon itself
		\*********************************************************************/
		int cDataSize	= words * planes * 2;
		
		rsc->cIcons[idx].icons[j].colData = (int16_t *) malloc(cDataSize);
		if (rsc->cIcons[idx].icons[j].colData == NULL)
			{
			WARN("Cannot alloc space for colour data for icon %d.%d", idx, j);
			return 0;
			}
			
		if (fread(rsc->cIcons[idx].icons[j].colData, cDataSize, 1, fp) != 1)
			{
			WARN("Cannot read info data for colour icon %d.%d", idx, j);
			return 0;
			}
		else
			{
			uint16_t *ptr = (uint16_t *)(rsc->cIcons[idx].icons[j].colData);
			for (int i=0; i<cDataSize/2; i++)
				{
				*ptr = ntohs(*ptr);
				ptr ++;
				}
			}
		
		/*********************************************************************\
		|* Read in the data and form the structure: icon mask
		\*********************************************************************/
		rsc->cIcons[idx].icons[j].colMask = (int16_t *) malloc(words*2);
		if (rsc->cIcons[idx].icons[j].colMask == NULL)
			{
			WARN("Cannot alloc space for colour mask for icon %d.%d", idx, j);
			return 0;
			}
		
		if (fread(rsc->cIcons[idx].icons[j].colMask, words*2, 1, fp) != 1)
			{
			WARN("Cannot read info data for colour mask %d.%d", idx, j);
			return 0;
			}
		else
			{
			uint16_t *ptr = (uint16_t *)(rsc->cIcons[idx].icons[j].colMask);
			for (int i=0; i<words; i++)
				{
				*ptr = ntohs(*ptr);
				ptr ++;
				}
			}

		if (selDataPresent)
			{
			/*****************************************************************\
			|* Read in the data and form the structure: selected-icon itself
			\*****************************************************************/
			rsc->cIcons[idx].icons[j].selData = (int16_t *) malloc(cDataSize);
			if (rsc->cIcons[idx].icons[j].selData == NULL)
				{
				WARN("Cannot alloc space for sel data for icon %d.%d", idx, j);
				return 0;
				}
			
			if (fread(rsc->cIcons[idx].icons[j].selData, cDataSize, 1, fp) != 1)
				{
				WARN("Cannot read data for sel data for icon %d.%d", idx, j);
				return 0;
				}
			else
				{
				uint16_t *ptr = (uint16_t *)(rsc->cIcons[idx].icons[j].selData);
				for (int i=0; i<cDataSize/2; i++)
					{
					*ptr = ntohs(*ptr);
					ptr ++;
					}
				}
				
			/*****************************************************************\
			|* Read in the data and form the structure: selected-icon mask
			\*****************************************************************/
			rsc->cIcons[idx].icons[j].selMask = (int16_t *) malloc(words*2);
			if (rsc->cIcons[idx].icons[j].selMask == NULL)
				{
				WARN("Cannot alloc space for sel mask for icon %d.%d", idx, j);
				return 0;
				}
			
			if (fread(rsc->cIcons[idx].icons[j].selMask, words*2, 1, fp) != 1)
				{
				WARN("Cannot read data for sel mask %d.%d", idx, j);
				return 0;
				}
			else
				{
				uint16_t *ptr = (uint16_t *)(rsc->cIcons[idx].icons[j].selMask);
				for (int i=0; i<words; i++)
					{
					*ptr = ntohs(*ptr);
					ptr ++;
					}
				}
			}
		else
			{
			rsc->cIcons[idx].icons[j].selData = NULL;
			rsc->cIcons[idx].icons[j].selMask = NULL;
			}
		}
	
	return 1;
	}

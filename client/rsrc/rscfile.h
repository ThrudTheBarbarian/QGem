//
//  rscfile.h
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#ifndef rscfile_h
#define rscfile_h

#include <stdio.h>
#include <stdint.h>

#include "gemobject.h"

/*****************************************************************************\
|* Define the header of the RSC file format.
|*
|* In the new format (rsh_vrsn == 4), the rsh_rssize value points to an
|* extension array, which is a series of LONG offsets. The first one is
|* always the true size of the RSC file, then there are any number of offsets,
|* but CICONBLK *always* occupies the first slot. CICONBLK is the only current
|* extension.
|*
|* The array of extension pointers is terminated by a LONG value of 0.
\*****************************************************************************/

typedef struct
	{
	uint16_t	rsh_vrsn;			/* Version, old = 0 or 1, new = 4		*/
	uint16_t 	rsh_object;			/* Offset to OBJECT structures			*/
	uint16_t 	rsh_tedinfo;		/* Offset to TEDINFO structures			*/
	uint16_t 	rsh_iconblk;		/* Offset to ICONBLK structures			*/
	uint16_t  	rsh_bitblk;			/* Offset to BITBLK structures			*/
	uint16_t 	rsh_frstr;			/* offset to string data				*/
	uint16_t 	rsh_string;    		/* Unused                            	*/
	uint16_t 	rsh_imdata;			/* offset to image data					*/
	uint16_t 	rsh_frimg;			/* offset to image pointer table		*/
	uint16_t	rsh_trindex;		/* offset to tree pointer table			*/
	uint16_t 	rsh_nobs;			/* number of OBJECTs in the file		*/
	uint16_t	rsh_ntree;			/* number of object trees in the file	*/
	uint16_t 	rsh_nted;			/* number of TEDINFOs in the file		*/
	uint16_t 	rsh_nib;			/* number of ICONBLKs in the file		*/
	uint16_t 	rsh_nbb;			/* number of BITBLKs in the file		*/
	uint16_t 	rsh_nstring;		/* number of free strings in the file	*/
	uint16_t 	rsh_nimages;		/* number of free images in the file	*/
	uint16_t	rsh_rssize;			/* Size of file (old) or offset to the	*/
									/*  extension arry (new)				*/
	} RscFileHeader;

/*****************************************************************************\
|* Define the internal representation of a coloured icon
|*
|* 'n' below  is defined as numPlanes * [words in mono icon]
\*****************************************************************************/
typedef struct
	{
	uint16_t	numPlanes;			/* Number of planes this icon is for	*/
	uint32_t	colData;			/* empty on disk, calculated on load	*/
	uint32_t	colMask;			/* empty on disk, calculated on load	*/
	uint32_t	selData;			/* must be !0 if selected data exists	*/
	uint32_t 	selMask;			/* calculated at load					*/
	int32_t		nextRes;			/* 1 if more icons to follow			*/
	int16_t *	colourData;			/* n words of colour data				*/
	int16_t *	colourMask;			/* n words of colout-mask data			*/
	int16_t *	selectData;			/* n words of select data				*/
	int16_t *	selectMask;			/* n words of select-mask data			*/
	} CIcon;
	
/*****************************************************************************\
|* Define the internal representation the coloured icons
\*****************************************************************************/
typedef struct
	{
	ICONBLK 	monoIcon;			/* The ICONBLk for the mono icon 		*/
	uint32_t	nCIcons;			/* Number of CICONS at different res	*/
	uint16_t *	monoData;			/* Monochrome bitmap data				*/
	uint16_t *	monoMask;			/* Monochrome bitmap mask				*/
	char		iconText[12];		/* Text for icon (max 11 chars)			*/
	CIcon **	icons;				/* Structure per resolution				*/
	} CICONBLK;
	
/*****************************************************************************\
|* Define the internal representation of an RSC file
\*****************************************************************************/

typedef struct
	{
	uint16_t	version;			/* Version, old = 0 or 1, new = 4		*/
	uint32_t	size;				/* File size							*/
	int 		nObjects;			/* Number of OBJECT structures 			*/
	OBJECT **	objects;			/* Pointers to each OBJECT structure 	*/
	int			nCicons;			/* Number of CICON structures			*/
	CICONBLK**	cIcons;				/* The actual CICON data				*/
	} RscFile;


/*****************************************************************************\
|* Read an RSC file into memory
\*****************************************************************************/
int resourceLoad(const char * filename, RscFile *result);

#endif /* rscfile_h */

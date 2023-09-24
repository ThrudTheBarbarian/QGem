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
|* Define the internal representation of an RSC file
\*****************************************************************************/

typedef struct
	{
	uint16_t	version;			/* Version, old = 0 or 1, new = 4		*/
	uint32_t	size;				/* File size							*/
	int 		nObjects;			/* Number of OBJECT structures 			*/
	OBJECT *	objects;			/* Pointer to each OBJECT structure 	*/
	int			nCicons;			/* Number of CICON structures			*/
	CICONBLK*	cIcons;				/* The actual CICON data				*/
	} RscFile;


/*****************************************************************************\
|* Read an RSC file into memory
\*****************************************************************************/
int resourceLoad(const char * filename, RscFile *result);

#endif /* rscfile_h */

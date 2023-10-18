//
//  rsrc_gaddr.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/17/23.
//

#include <stdio.h>

#include "aes.h"
#include "gemTypes.h"
#include "macros.h"
#include "rscfile.h"

/*****************************************************************************\
|* Function prototypes
\*****************************************************************************/
static int16_t _findTree(RscFile *rsc, int16_t index, OBJECT **address);
static int16_t _findObject(RscFile *rsc, int16_t index, OBJECT *address);
static int16_t _findTedInfo(RscFile *rsc, int16_t index, TEDINFO *address);
static int16_t _findIconBlk(RscFile *rsc, int16_t index, ICONBLK *address);
static int16_t _findBitBlk(RscFile *rsc, int16_t index, BITBLK *address);
static int16_t _findString(RscFile *rsc, int16_t index, char **address);
static int16_t _findImageData(RscFile *rsc, int16_t index, BITBLK *address);
static int16_t _findObSpec(RscFile *rsc, int16_t index, void **address);
static int16_t _findTepText(RscFile *rsc, int16_t index, int8_t **address);
static int16_t _findTepTmplt(RscFile *rsc, int16_t index, int8_t **address);
static int16_t _findTepValid(RscFile *rsc, int16_t index, int8_t **address);
static int16_t _findIBMask(RscFile *rsc, int16_t index, uint16_t **address);
static int16_t _findIBData(RscFile *rsc, int16_t index, uint16_t **address);
static int16_t _findIBText(RscFile *rsc, int16_t index, int8_t **address);
static int16_t _findBBData(RscFile *rsc, int16_t index, uint16_t **address);

/*****************************************************************************\
|*  7801 : rsrc_gaddr()
|*		 : Obtain a pointer to various object structures in the resource
\*****************************************************************************/
int16_t rsrc_gaddr(int16_t type, int16_t index, void *address)
	{
	/*************************************************************************\
	|*  Do some input validation
	\*************************************************************************/
	if ((type < R_TREE) || (type >= R_MAX) || (index < 0))
		return 0;
	
	/*************************************************************************\
	|*  Check we have a resource file
	\*************************************************************************/
	RscFile *rsc = _rsrcGet();
	if (rsc == NULL)
		return 0;
	
	int ok = 0;
	switch (type)
		{
		case R_TREE:
			ok = _findTree(rsc, index, (OBJECT**)address);
			break;
		
		case R_OBJECT:
			ok = _findObject(rsc, index, (OBJECT*)address);
			break;
		
		case R_TEDINFO:
			ok = _findTedInfo(rsc, index, (TEDINFO*)address);
			break;
		
		case R_ICONBLK:
			ok = _findIconBlk(rsc, index, (ICONBLK*)address);
			break;
		
		case R_BITBLK:
			ok = _findBitBlk(rsc, index, (BITBLK*)address);
			break;
		
		case R_STRING:
			ok = _findString(rsc, index, (char**)address);
			break;
		
		case R_IMAGEDATA:
			ok = _findImageData(rsc, index, (BITBLK*)address);
			break;
		
		case R_OBSPEC:
			ok = _findObSpec(rsc, index, (void **)address);
			break;
		
		case R_TEPTEXT:
			ok = _findTepText(rsc, index, (int8_t **)address);
			break;
		
		case R_TEPTMPLT:
			ok = _findTepTmplt(rsc, index, (int8_t **)address);
			break;
		
		case R_TEPVALID:
			ok = _findTepValid(rsc, index, (int8_t **)address);
			break;
		
		case R_IBPMASK:
			ok = _findIBMask(rsc, index, (uint16_t **)address);
			break;
		
		case R_IBPDATA:
			ok = _findIBData(rsc, index, (uint16_t **)address);
			break;
		
		case R_IBPTEXT:
			ok = _findIBText(rsc, index, (int8_t **)address);
			break;
		
		case R_BIPDATA:
			ok = _findBBData(rsc, index, (uint16_t **)address);
			break;
		
		case R_FRSTR:
			*((char ***)address) = rsc->strings;
			ok = 1;
			break;
		
		case R_FRIMG:
			*((BITBLK **)address) = rsc->images;
			ok = 1;
			break;
		
		default:
			WARN("Unknown resource-type %d", type);
			break;
		}
	
	return ok;
	}

/*****************************************************************************\
|* Find an object TREE
\*****************************************************************************/
static int16_t _findTree(RscFile *rsc, int16_t index, OBJECT **address)
	{
	if (index >= rsc->nTrees)
		return 0;
	
	*address = rsc->trees[index];
	return 1;
	}

/*****************************************************************************\
|* Find an OBJECT
\*****************************************************************************/
static int16_t _findObject(RscFile *rsc, int16_t index, OBJECT *address)
	{
	if (index >= rsc->nObjects)
		return 0;
	
	*address = rsc->objects[index];
	return 1;
	}

/*****************************************************************************\
|* Find a TEDINFO
\*****************************************************************************/
static int16_t _findTedInfo(RscFile *rsc, int16_t index, TEDINFO *address)
	{
	if (index >= rsc->nTedInfo)
		return 0;
	
	*address = rsc->tedInfo[index];
	return 1;
	}

/*****************************************************************************\
|* Find an ICONBLK
\*****************************************************************************/
static int16_t _findIconBlk(RscFile *rsc, int16_t index, ICONBLK *address)
	{
	if (index >= rsc->nIconBlks)
		return 0;
	
	*address = rsc->iconBlks[index];
	return 1;
	}

/*****************************************************************************\
|* Find a BITBLK
\*****************************************************************************/
static int16_t _findBitBlk(RscFile *rsc, int16_t index, BITBLK *address)
	{
	if (index >= rsc->nBitBlks)
		return 0;
	
	*address = rsc->bitBlks[index];
	return 1;
	}

/*****************************************************************************\
|* Find a string
\*****************************************************************************/
static int16_t _findString(RscFile *rsc, int16_t index, char **address)
	{
	if (index >= rsc->nStrings)
		return 0;
	
	*address = rsc->strings[index];
	return 1;
	}

/*****************************************************************************\
|* Find Image data
\*****************************************************************************/
static int16_t _findImageData(RscFile *rsc, int16_t index, BITBLK *address)
	{
	if (index >= rsc->nImages)
		return 0;
	
	*address = rsc->images[index];
	return 1;
	}

/*****************************************************************************\
|* Find the ob_spec field in an OBJECT
\*****************************************************************************/
static int16_t _findObSpec(RscFile *rsc, int16_t index, void **address)
	{
	int ok		= 0;
	OBJECT *obj = NULL;
	if (_findObject(rsc, index, obj) > 0)
		{
		ok = 1;
		*address = obj->ob_spec;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the text in a TEDINFO
\*****************************************************************************/
static int16_t _findTepText(RscFile *rsc, int16_t index, int8_t **address)
	{
	int ok		= 0;
	TEDINFO *ted = NULL;
	if (_findTedInfo(rsc, index, ted) > 0)
		{
		ok = 1;
		*address = ted->te_ptext;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the template text in a TEDINFO
\*****************************************************************************/
static int16_t _findTepTmplt(RscFile *rsc, int16_t index, int8_t **address)
	{
	int ok		= 0;
	TEDINFO *ted = NULL;
	if (_findTedInfo(rsc, index, ted) > 0)
		{
		ok = 1;
		*address = ted->te_ptmplt;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the validation text in a TEDINFO
\*****************************************************************************/
static int16_t _findTepValid(RscFile *rsc, int16_t index, int8_t **address)
	{
	int ok		= 0;
	TEDINFO *ted = NULL;
	if (_findTedInfo(rsc, index, ted) > 0)
		{
		ok = 1;
		*address = ted->te_pvalid;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the pointer to the mask data in an ICONBLK
\*****************************************************************************/
static int16_t _findIBMask(RscFile *rsc, int16_t index, uint16_t **address)
	{
	int ok		= 0;
	ICONBLK *blk = NULL;
	if (_findIconBlk(rsc, index, blk) > 0)
		{
		ok = 1;
		*address = blk->ib_pmask;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the pointer to the image data in an ICONBLK
\*****************************************************************************/
static int16_t _findIBData(RscFile *rsc, int16_t index, uint16_t **address)
	{
	int ok		= 0;
	ICONBLK *blk = NULL;
	if (_findIconBlk(rsc, index, blk) > 0)
		{
		ok = 1;
		*address = blk->ib_pdata;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the pointer to the text in an ICONBLK
\*****************************************************************************/
static int16_t _findIBText(RscFile *rsc, int16_t index, int8_t **address)
	{
	int ok		= 0;
	ICONBLK *blk = NULL;
	if (_findIconBlk(rsc, index, blk) > 0)
		{
		ok = 1;
		*address = blk->ib_ptext;
		}
	return ok;
	}

/*****************************************************************************\
|* Find the pointer to the text in an ICONBLK
\*****************************************************************************/
static int16_t _findBBData(RscFile *rsc, int16_t index, uint16_t **address)
	{
	int ok		= 0;
	BITBLK *blk = NULL;
	if (_findBitBlk(rsc, index, blk) > 0)
		{
		ok = 1;
		*address = blk->bi_pdata;
		}
	return ok;
	}

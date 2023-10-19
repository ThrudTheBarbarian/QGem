//
//  rsrc_saddr.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/18/23.
//

// FIXME: The freeing of memory is not complete. At all.

#include <stdio.h>

#include "aes.h"
#include "gemTypes.h"
#include "macros.h"
#include "rscfile.h"

/*****************************************************************************\
|* Function prototypes
\*****************************************************************************/
static int16_t _setTree(RscFile *rsc, int16_t index, OBJECT *address);
static int16_t _setObject(RscFile *rsc, int16_t index, OBJECT *address);
static int16_t _setTedInfo(RscFile *rsc, int16_t index, TEDINFO *address);
static int16_t _setIconBlk(RscFile *rsc, int16_t index, ICONBLK *address);
static int16_t _setBitBlk(RscFile *rsc, int16_t index, BITBLK *address);
static int16_t _setString(RscFile *rsc, int16_t index, char *address);
static int16_t _setImageData(RscFile *rsc, int16_t index, BITBLK *address);
static int16_t _setObSpec(RscFile *rsc, int16_t index, void *address);
static int16_t _setTepText(RscFile *rsc, int16_t index, int8_t *address);
static int16_t _setTepTmplt(RscFile *rsc, int16_t index, int8_t *address);
static int16_t _setTepValid(RscFile *rsc, int16_t index, int8_t *address);
static int16_t _setIBMask(RscFile *rsc, int16_t index, uint16_t *address);
static int16_t _setIBData(RscFile *rsc, int16_t index, uint16_t *address);
static int16_t _setIBText(RscFile *rsc, int16_t index, int8_t *address);
static int16_t _setBBData(RscFile *rsc, int16_t index, uint16_t *address);
static int16_t _setBBData(RscFile *rsc, int16_t index, uint16_t *address);
static int16_t setStringPointer(RscFile *rsc, int16_t index, char **address);
static int16_t setImagePointer(RscFile *rsc, int16_t index, BITBLK *address);


/*****************************************************************************\
|*  7805 : rsrc_saddr()
|*		 : Set the pointer for an object structure into the RSC file. This
|* 		 : will make the RSC file take ownership of the memory
\*****************************************************************************/
int16_t rsrc_saddr(int16_t type, int16_t index, void *address)
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
			ok = _setTree(rsc, index, (OBJECT*)address);
			break;
		
		case R_OBJECT:
			ok = _setObject(rsc, index, (OBJECT*)address);
			break;
		
		case R_TEDINFO:
			ok = _setTedInfo(rsc, index, (TEDINFO*)address);
			break;
		
		case R_ICONBLK:
			ok = _setIconBlk(rsc, index, (ICONBLK*)address);
			break;
		
		case R_BITBLK:
			ok = _setBitBlk(rsc, index, (BITBLK*)address);
			break;
		
		case R_STRING:
			ok = _setString(rsc, index, (char*)address);
			break;
		
		case R_IMAGEDATA:
			ok = _setImageData(rsc, index, (BITBLK*)address);
			break;
		
		case R_OBSPEC:
			ok = _setObSpec(rsc, index, address);
			break;
		
		case R_TEPTEXT:
			ok = _setTepText(rsc, index, (int8_t *)address);
			break;
		
		case R_TEPTMPLT:
			ok = _setTepTmplt(rsc, index, (int8_t *)address);
			break;
		
		case R_TEPVALID:
			ok = _setTepValid(rsc, index, (int8_t *)address);
			break;
		
		case R_IBPMASK:
			ok = _setIBMask(rsc, index, (uint16_t *)address);
			break;
		
		case R_IBPDATA:
			ok = _setIBData(rsc, index, (uint16_t *)address);
			break;
		
		case R_IBPTEXT:
			ok = _setIBText(rsc, index, (int8_t *)address);
			break;
		
		case R_BIPDATA:
			ok = _setBBData(rsc, index, (uint16_t *)address);
			break;
		
		case R_FRSTR:
			ok = setStringPointer(rsc, index, (char **)address);
			break;
		
		case R_FRIMG:
			ok = setImagePointer(rsc, index, (BITBLK *)address);
			break;
		
		default:
			WARN("Unknown resource-type %d", type);
			break;
		}
	
	return ok;
	}


/*****************************************************************************\
|* Set an object TREE
\*****************************************************************************/
static int16_t _setTree(RscFile *rsc, int16_t index, OBJECT *address)
	{
	if (index >= rsc->nTrees)
		return 0;
	
	rsc->trees[index] = address;
	return 1;
	}

/*****************************************************************************\
|* Set an OBJECT
\*****************************************************************************/
static int16_t _setObject(RscFile *rsc, int16_t index, OBJECT *address)
	{
	if (index >= rsc->nObjects)
		return 0;
	
	rsc->objects[index] = *address;
	return 1;
	}

/*****************************************************************************\
|* Set a TEDINFO
\*****************************************************************************/
static int16_t _setTedInfo(RscFile *rsc, int16_t index, TEDINFO *address)
	{
	if (index >= rsc->nTedInfo)
		return 0;
	
	rsc->tedInfo[index] = *address;
	return 1;
	}

/*****************************************************************************\
|* Set an ICONBLK
\*****************************************************************************/
static int16_t _setIconBlk(RscFile *rsc, int16_t index, ICONBLK *address)
	{
	if (index >= rsc->nIconBlks)
		return 0;
	
	rsc->iconBlks[index] = *address;
	return 1;
	}

/*****************************************************************************\
|* Set a BITBLK
\*****************************************************************************/
static int16_t _setBitBlk(RscFile *rsc, int16_t index, BITBLK *address)
	{
	if (index >= rsc->nBitBlks)
		return 0;
	
	rsc->bitBlks[index] = *address;
	return 1;
	}

/*****************************************************************************\
|* Set a string
\*****************************************************************************/
static int16_t _setString(RscFile *rsc, int16_t index, char *address)
	{
	if (index >= rsc->nStrings)
		return 0;
	
	free(rsc->strings[index]);
	rsc->strings[index] = address;
	return 1;
	}

/*****************************************************************************\
|* Set Image data
\*****************************************************************************/
static int16_t _setImageData(RscFile *rsc, int16_t index, BITBLK *address)
	{
	if (index >= rsc->nImages)
		return 0;
	
	rsc->images[index] = *address;
	return 1;
	}

/*****************************************************************************\
|* Set the ob_spec field in an OBJECT
\*****************************************************************************/
static int16_t _setObSpec(RscFile *rsc, int16_t index, void *address)
	{
	int ok		= 0;
	if (index < rsc->nObjects)
		{
		ok = 1;
		rsc->objects[index].ob_spec = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the text in a TEDINFO
\*****************************************************************************/
static int16_t _setTepText(RscFile *rsc, int16_t index, int8_t *address)
	{
	int ok		= 0;
	if (index < rsc->nTedInfo)
		{
		TEDINFO *ted = &(rsc->tedInfo[index]);
		ok = 1;
		free(ted->te_ptext);
		ted->te_ptext = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the template text in a TEDINFO
\*****************************************************************************/
static int16_t _setTepTmplt(RscFile *rsc, int16_t index, int8_t *address)
	{
	int ok		= 0;
	if (index < rsc->nTedInfo)
		{
		TEDINFO *ted = &(rsc->tedInfo[index]);
		ok = 1;
		free(ted->te_ptmplt);
		ted->te_ptmplt = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the validation text in a TEDINFO
\*****************************************************************************/
static int16_t _setTepValid(RscFile *rsc, int16_t index, int8_t *address)
	{
	int ok		= 0;
	if (index < rsc->nTedInfo)
		{
		TEDINFO *ted = &(rsc->tedInfo[index]);
		ok = 1;
		free(ted->te_pvalid);
		ted->te_pvalid = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the pointer to the mask data in an ICONBLK
\*****************************************************************************/
static int16_t _setIBMask(RscFile *rsc, int16_t index, uint16_t *address)
	{
	int ok		= 0;
	if (index < rsc->nIconBlks)
		{
		ICONBLK *blk = &(rsc->iconBlks[index]);
		ok = 1;
		free(blk->ib_pmask);
		blk->ib_pmask = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the pointer to the image data in an ICONBLK
\*****************************************************************************/
static int16_t _setIBData(RscFile *rsc, int16_t index, uint16_t *address)
	{
	int ok		= 0;
	if (index < rsc->nIconBlks)
		{
		ICONBLK *blk = &(rsc->iconBlks[index]);
		ok = 1;
		free(blk->ib_pdata);
		blk->ib_pdata = address;
		}
	return ok;
	}

/*****************************************************************************\
|* Set the pointer to the text in an ICONBLK
\*****************************************************************************/
static int16_t _setIBText(RscFile *rsc, int16_t index, int8_t *address)
	{
	int ok		= 0;
	if (index < rsc->nIconBlks)
		{
		ICONBLK *blk = &(rsc->iconBlks[index]);
		ok = 1;
		free(blk->ib_ptext);
		blk->ib_ptext = address;
		}
	return ok;
	}


/*****************************************************************************\
|* Set the pointer to the data in an BITBLK
\*****************************************************************************/
static int16_t _setBBData(RscFile *rsc, int16_t index, uint16_t *address)
	{
	int ok		= 0;
	if (index < rsc->nBitBlks)
		{
		BITBLK *blk = &(rsc->bitBlks[index]);
		ok = 1;
		free(blk->bi_pdata);
		blk->bi_pdata = address;
		}
	return ok;
	}
	
/*****************************************************************************\
|* Set the pointer to string pointers table
\*****************************************************************************/
static int16_t setStringPointer(RscFile *rsc, int16_t index, char **address)
	{
	rsc->strings = address;
	return 1;
	}
	
/*****************************************************************************\
|* Set the pointer to image pointers table
\*****************************************************************************/
static int16_t setImagePointer(RscFile *rsc, int16_t index, BITBLK *address)
	{
	rsc->images = address;
	return 1;
	}

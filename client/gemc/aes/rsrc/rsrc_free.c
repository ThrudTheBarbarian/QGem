//
//  rsrc_free.c
//  gemc
//
//  Created by Simon Gornall on 10/11/23.
//

#include <stdio.h>

#include "aes.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "rscfile.h"

/*****************************************************************************\
|*  7800 : rsrc_free()
|*		 : Free up the resources used by a resource file
\*****************************************************************************/
int16_t rsrc_free(void)
	{
	int16_t ok = 0;
	
	RscFile *rsc = _rsrcGet();
	if (rsc == NULL)
		return 1;
	
	/*************************************************************************\
	|* Objects all point to things within the header, or arent pointers, so
	|* can be freed en-masse
	\*************************************************************************/
	free(rsc->objects);
	
	/*************************************************************************\
	|* Colour icons need their data to be freed within the structure, so do
	|* them individually
	\*************************************************************************/
	for (int i=0; i<rsc->nCicons; i++)
		{
		CICON *iconPtr = &(rsc->cIcons->icons[i]);
		while (iconPtr)
			{
			CICON *next = iconPtr->next;
			free(iconPtr->colData);
			free(iconPtr->colMask);
			free(iconPtr->selData);
			free(iconPtr->selMask);
			free(iconPtr);
			iconPtr = next;
			}
		}
	free(rsc->cIcons);
	
	/*************************************************************************\
	|* TEDINFO's need their data to be freed within the structure, so do
	|* them individually
	\*************************************************************************/
	for (int i=0; i<rsc->nTedInfo; i++)
		{
		TEDINFO *ted = &(rsc->tedInfo[i]);
		if (ted)
			{
			free(ted->te_ptext);
			free(ted->te_ptmplt);
			free(ted->te_pvalid);
			}
		}
	free(rsc->tedInfo);
	
	/*************************************************************************\
	|* iCONBLK's need their data to be freed within the structure, so do
	|* them individually
	\*************************************************************************/
	for (int i=0; i<rsc->nIconBlks; i++)
		{
		ICONBLK *iblk = &(rsc->iconBlks[i]);
		if (iblk)
			{
			free(iblk->ib_pmask);
			free(iblk->ib_pdata);
			free(iblk->ib_ptext);
			}
		}
	free(rsc->iconBlks);
	
	/*************************************************************************\
	|* BITBLK's need their data to be freed within the structure, so do
	|* them individually
	\*************************************************************************/
	for (int i=0; i<rsc->nBitBlks; i++)
		{
		BITBLK *bblk = &(rsc->bitBlks[i]);
		if (bblk)
			free(bblk->bi_pdata);
		}
	free(rsc->bitBlks);

	/*************************************************************************\
	|* strings's need their data to be freed, so do them individually
	\*************************************************************************/
	for (int i=0; i<rsc->nStrings; i++)
		free(rsc->strings[i]);
	free(rsc->strings);

	
	/*************************************************************************\
	|* Images are the same as BITBLKs
	\*************************************************************************/
	for (int i=0; i<rsc->nImages; i++)
		{
		BITBLK *bblk = &(rsc->images[i]);
		if (bblk)
			free(bblk->bi_pdata);
		}
	free(rsc->images);
	
	/*************************************************************************\
	|* Trees are just links to objects
	\*************************************************************************/
	free(rsc->trees);

	/*************************************************************************\
	|* Free the top-level structure
	\*************************************************************************/
	free(rsc);
	
	return ok;
	}

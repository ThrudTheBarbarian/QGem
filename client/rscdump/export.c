//
//  export.c
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/24/23.
//

#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "export.h"
#include "gem.h"
#include "gemobject.h"
#include "pam.h"
#include "vdi.h"

extern char *objType[];

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
int _exportIcon(OBJECT *o, const char *name);
int _exportCIcon(OBJECT *o, const char *name);

/*****************************************************************************\
|* Export an object to a file. This is basically a function to figure out an
|* appropriate filename before passing on to exportObjectTo()
\*****************************************************************************/
int exportObject(OBJECT *o)
	{
	char name[PATH_MAX];
	
	switch (o->ob_type)
		{
		case G_BOX:
		case G_IBOX:
		case G_TEXT:
		case G_FTEXT:
		case G_BOXTEXT:
		case G_FBOXTEXT:
		case G_BOXCHAR:
		case G_IMAGE:
		case G_BUTTON:
		case G_STRING:
		case G_TITLE:
		case G_SWBUTTON:
		case G_POPUP:
			sprintf(name, "%s_%d_%d", objType[o->ob_type], o->ob_x, o->ob_y);
			strcpy(name, objType[o->ob_type]);
			break;
		
		case G_ICON:
			sprintf(name, "G_ICON_%s", ((ICONBLK *) o->ob_spec)->ib_ptext);
			break;

		case G_CICON:
			sprintf(name, "G_CICON_%s",
					((CICONBLK *) o->ob_spec)->monoIcon.ib_ptext);
			break;
		}
	
	return exportObjectTo(o, name);
	}

/*****************************************************************************\
|* Export an object to a named file. The name is used as a stem for any outputs
\*****************************************************************************/
int exportObjectTo(OBJECT *o, const char *name)
	{
	int ok = 0;
	switch (o->ob_type)
		{
		case G_ICON:
			ok = _exportIcon(o, name);
			break;
		case G_CICON:
			ok = _exportCIcon(o, name);
			break;
		}
		
	return ok;
	}


/*****************************************************************************\
|* Export an icon to a named file. In this case a PAM file. We use
|* BLACKANDWHITE for bitmaps without a mask, and BLACKANDWHITE_ALPHA for
|* bitmaps with a mask. If there's a selection entry, we write that out
|* as a separate file
\*****************************************************************************/
int _exportIcon(OBJECT *o, const char *name)
	{
	ICONBLK *blk = (ICONBLK *) o->ob_spec;
	
	/*************************************************************************\
	|* Create an MFDB for the source bitmap
	\*************************************************************************/
	MFDB src;
	src.fd_addr 	= blk->ib_pdata;
	src.fd_w		= blk->ib_wicon;
	src.fd_h		= blk->ib_hicon;
	src.fd_wdwidth	= (src.fd_w / 16) + ((src.fd_w % 15) != 0 ? 1 : 0);
	src.fd_stand	= MFDB_STANDARD;
	src.fd_nplanes	= 1;
	
	MFDB dst;
	memset(&dst, 0, sizeof(MFDB));
	
	vr_trnfm(0, &src, &dst);
	
	// dst is now in chunky form, so store it to a PAM file
	
	return 1;
	}

/*****************************************************************************\
|* Export an icon to a named file. In this case a PAM file. We use
|* RGB for icons without a mask, and RGB_ALPHA for icons with a mask. If
|* there's a selection entry, we write that out as a separate file
\*****************************************************************************/
int _exportCIcon(OBJECT *o, const char *name)
	{
	CICONBLK *blk = (CICONBLK *) o->ob_spec;
	
	/*************************************************************************\
	|* For each icon-image in this object
	\*************************************************************************/
	CICON * icon	= blk->icons;
	int count 		= 0;
	while (icon != NULL)
		{
		/*********************************************************************\
		|* Create an MFDB for each source bitmap
		\*********************************************************************/
		MFDB src;
		src.fd_addr 	= icon->colData;
		src.fd_w		= blk->monoIcon.ib_wicon;
		src.fd_h		= blk->monoIcon.ib_hicon;
		src.fd_wdwidth	= ((icon->numPlanes * src.fd_w) / 16)
						+ ((src.fd_w & 15) != 0 ? 1 : 0);
		src.fd_stand	= MFDB_STANDARD;
		src.fd_nplanes	= icon->numPlanes;
		
		/*********************************************************************\
		|* Create an empty MFDB for the result, and transform to 'chunky'
		\*********************************************************************/
		MFDB imgData;
		memset(&imgData, 0, sizeof(MFDB));
		vr_trnfm(0, &src, &imgData);
		
		/*********************************************************************\
		|* If there's a mask, fetch that too
		\*********************************************************************/
		MFDB imgMask;
		memset(&imgMask, 0, sizeof(MFDB));
		if (icon->colMask != NULL)
			{
			src.fd_addr = icon->colMask;
			vr_trnfm(0, &src, &imgMask);
			}
			
		/*********************************************************************\
		|* Create the PAM file
		\*********************************************************************/
		char filename[PATH_MAX];
		if ((count == 0) && (icon->next == NULL))
			strcpy(filename, name);
		else
			sprintf(filename, "%s_%d", name, count);
			
		writePAM(filename, &imgData, &imgMask);
			
		/*********************************************************************\
		|* Get the next icon if there is one
		\*********************************************************************/
		count ++;
		icon = icon->next;
		}
	
	
	return 1;
	}

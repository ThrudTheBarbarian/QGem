//
//  rsrc_obfix.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/18/23.
//

#include "aes.h"
#include "gemio.h"
#include "gemmsg.h"
#include "macros.h"
#include "rscfile.h"
#include "vdi.h"

#undef DEBUG_OBFIX

#ifdef DEBUG_OBFIX
static void objdump(OBJECT *o)
	{
	printf("(%4d,%4d)+->(%4d,%4d)", o->ob_x, o->ob_y,
			o->ob_width, o->ob_height);
	}
#endif

/*****************************************************************************\
|*  7803 : rsrc_obfix()
|*		 : Convert an object from character-based to pixel-based
\*****************************************************************************/
int16_t rsrc_obfix(OBJECT *tree, int16_t object)
	{
	/*************************************************************************\
	|* Check to see if we have an RSC file
	\*************************************************************************/
	RscFile *rsc = _rsrcGet();
	if (rsc == NULL)
		return 1;

	/*************************************************************************\
	|* Check to see if tree = NULL
	\*************************************************************************/
	if (tree == NULL)
		{
		WARN("Passed NULL tree to rsrc_obfix for object %d", object);
		return 1;
		}
		
	/*************************************************************************\
	|* Get the correct object
	\*************************************************************************/
	OBJECT *toDo = tree + object;
	
	#ifdef DEBUG_OBFIX
		objdump(toDo);
		printf(" -> ");
	#endif
	
	/*************************************************************************\
	|* Get the screen width & height, as well as char width and height
	\*************************************************************************/
	int16_t * ws 	= v_opnvwkParams();
	int screenW 	= ws[0];
	int boxW		= ws[59];
	int boxH	 	= ws[60];
		
	/*************************************************************************\
	|* If the object's width is precisely 80, then it gets the full screen
	|* width. This is a ridiculous protocol, when they had -ve numbers to
	|* play with [sigh]
	\*************************************************************************/
	if (toDo->ob_width == 80)
		{
		toDo->ob_width = screenW;
		toDo->ob_x = 0;
		}
	else
		{
		int 	chars 	= toDo->ob_width & 0xFF;
		int8_t	delta 	= toDo->ob_width >> 8;
		toDo->ob_width	= chars * boxW + delta;

		chars			= toDo->ob_x & 0xFF;
		delta			= toDo->ob_x >> 8;
		toDo->ob_x		= chars * boxW + delta;

		chars			= toDo->ob_height & 0xFF;
		delta			= toDo->ob_height >> 8;
		toDo->ob_height	= chars * boxH + delta;

		chars			= toDo->ob_y & 0xFF;
		delta			= toDo->ob_y >> 8;
		toDo->ob_y		= chars * boxH + delta;
		}
	
	#ifdef DEBUG_OBFIX
		objdump(toDo);
		printf("\n");
	#endif
	
	return 1;
	}

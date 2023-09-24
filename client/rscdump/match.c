//
//  match.c
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/23/23.
//

#include <stdio.h>
#include <string.h>

#include "match.h"
#include "gemobject.h"

/*****************************************************************************\
|* See if a pair of co-ords was provided
\*****************************************************************************/
static int _matchXY(const char *match, int *x, int *y)
	{
	int ok = 0;
	int X, Y;
	
	if (sscanf(match, "%d,%d", &X, &Y) == 2)
		{
		ok = 1;
		if (x != NULL)
			*x = X;
		if (y != NULL)
			*y = Y;
		}
	
	return ok;
	}

/*****************************************************************************\
|* Match a co-ordinate within the object
\*****************************************************************************/
static int _matchLocation(OBJECT *box, const char *match)
	{
	int result = 0;
	int x,y;
	
	if (_matchXY(match, &x, &y))
		if ((box->ob_x >= x) && (box->ob_x + box->ob_width < x))
			if ((box->ob_y >= y) && (box->ob_y + box->ob_height < y))
				result = 1;
	
	return result;
	}
	
/*****************************************************************************\
|* Match a G_BOX
\*****************************************************************************/
static int _matchBox(OBJECT *box, const char *match)
	{
	return _matchLocation(box, match);
	}

/*****************************************************************************\
|* Match a G_TEXT
\*****************************************************************************/
static int _matchGraphicText(OBJECT *box, const char *match)
	{
	int result = 0;
	
	// FIXME: Need a TEDINFO object first
	return result;
	}

/*****************************************************************************\
|* Match a G_STRING
\*****************************************************************************/
static int _matchText(OBJECT *o, const char *match)
	{
	int result = 0;
	
	if (strcasestr(o->ob_spec, match) != NULL)
		result = 1;
		
	return result;
	}
	
/*****************************************************************************\
|* Match a G_ICON
\*****************************************************************************/
static int _matchIcon(OBJECT *o, const char *match)
	{
	int result = 0;
	
	ICONBLK *icon = (ICONBLK *) o->ob_spec;
	if (strcasestr((const char *)(icon->ib_ptext), match) != NULL)
		result = 1;
		
	return result;
	}
	
/*****************************************************************************\
|* Match a G_CICON
\*****************************************************************************/
static int _matchCIcon(OBJECT *o, const char *match)
	{
	int result = 0;
	
	CICONBLK *icon = (CICONBLK *) o->ob_spec;
	if (strcasestr((const char *)(icon->monoIcon.ib_ptext), match) != NULL)
		result = 1;
		
	return result;
	}

/*****************************************************************************\
|* Match a C_SWBUTTON
\*****************************************************************************/
static int _matchSWInfo(OBJECT *o, const char *match)
	{
	int result = 0;
	
	SWINFO *info = (SWINFO *)o->ob_spec;
	if (strcasestr((const char *)(info->string), match) != NULL)
		result = 1;
		
	return result;
	}

/*****************************************************************************\
|* Match a G_POPUP
\*****************************************************************************/
static int _matchPopup(OBJECT *o, const char *match)
	{
	int result = 0;
	
	// FIXME: Needs POPINFO struct and tree-descent

	return result;
	}

/*****************************************************************************\
|* Determine, based on object type, whether an object matches a criterion
\*****************************************************************************/
int match(OBJECT *obj, const char *match)
	{
	int result = 0;
	
	switch (obj->ob_type)
		{
		case G_BOX:
		case G_IBOX:
			result = _matchBox(obj, match);
			break;
		
		case G_TEXT:
		case G_FTEXT:
			result = _matchGraphicText(obj, match);
			break;
		
		case G_BOXTEXT:
		case G_FBOXTEXT:
		case G_BOXCHAR:
			result = _matchBox(obj, match) | _matchText(obj, match);
			break;
		
		case G_IMAGE:
			result = _matchLocation(obj, match);
			break;
		
		case G_BUTTON:
			result = _matchLocation(obj, match);
			break;
		
		case G_STRING:
		case G_TITLE:
			result = _matchText(obj, match);
			break;
		
		case G_ICON:
			result = _matchLocation(obj, match) | _matchIcon(obj, match);
			break;

		case G_CICON:
			result = _matchLocation(obj, match) | _matchCIcon(obj, match);
			break;

		case G_SWBUTTON:
			result = _matchLocation(obj, match) | _matchSWInfo(obj, match);
			break;

		case G_POPUP:
			result = _matchLocation(obj, match) | _matchPopup(obj, match);
			break;
		}
	
	return result;
	}

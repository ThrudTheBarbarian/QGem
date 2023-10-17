//
//  listing.c
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/23/23.
//

#include <string.h>

char *objType[] = {
	"#0",  "#1",  "#2",  "#3",  "#4",  "#5",  "#6",  "#7",  "#8",  "#9",
	"#10", "#11", "#12", "#13", "#14", "#15", "#16", "#17", "#18", "#19",
	"G_BOX", "G_TEXT", "G_BOXTEXT", "G_IMAGE", "G_USERDEF",
	"G_IBOX", "G_BUTTON", "G_BOXCHAR", "G_STRING", "G_FTEXT",
	"G_FBOXTEXT", "G_ICON", "G_TITLE", "G_CICON", "G_SWBUTTON",
	"G_POPUP", "G_WINTITLE", "G_EDIT", "G_SHORTCUT", "G_SLIST"
	};


#include "listing.h"
#include "gemobject.h"

/*****************************************************************************\
|* Do the generic stuff
\*****************************************************************************/
static void _listGenericObject(OBJECT *o)
	{
	char xywh[128];
	sprintf(xywh, "(%d,%d +-> %d,%d)",
			o->ob_x, o->ob_y, o->ob_width, o->ob_height);
	printf("%-11s: %-25s ", objType[o->ob_type], xywh);
	}

/*****************************************************************************\
|* Match a G_BOX / G_IBOX
\*****************************************************************************/
static void _listBox(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_TEXT / G_FTEXT
\*****************************************************************************/
static void _listGraphicText(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BOXTEXT / G_FBOXTEXT / G_BOXCHAR
\*****************************************************************************/
static void _listBoxText(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_IMAGE
\*****************************************************************************/
static void _listImage(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listButton(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listText(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listIcon(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listCIcon(OBJECT *o)
	{
	_listGenericObject(o);
	CICONBLK *icon = (CICONBLK *) o->ob_spec;

	char tmp[128];
	char planes[1024];
	planes[0] = '\0';
	CICON *icons = icon->icons;
	char *comma = "";
	while (icons)
		{
		sprintf(tmp, "%s%d", comma, icons->numPlanes);
		comma = ",";
		strcat(planes, tmp);
		icons = icons->next;
		}
	printf("[%-12s:%dx%d, planes:%-12s]", icon->monoIcon.ib_ptext
						  , icon->monoIcon.ib_wicon
						  , icon->monoIcon.ib_hicon
						  , planes);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listSWButton(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Match a G_BUTTON
\*****************************************************************************/
static void _listPopup(OBJECT *o)
	{
	_listGenericObject(o);
	printf("\n");
	}

/*****************************************************************************\
|* Dump an object to stdout
\*****************************************************************************/
void listObject(OBJECT *o)
	{
	switch (o->ob_type)
		{
		case G_BOX:
		case G_IBOX:
			_listBox(o);
			break;
		
		case G_TEXT:
		case G_FTEXT:
			_listGraphicText(o);
			break;

		case G_BOXTEXT:
		case G_FBOXTEXT:
		case G_BOXCHAR:
			_listBoxText(o);
			break;

		case G_IMAGE:
			_listImage(o);
			break;

		case G_BUTTON:
			_listButton(o);
			break;

		case G_STRING:
		case G_TITLE:
			_listText(o);
			break;
		
		case G_ICON:
			_listIcon(o);
			break;

		case G_CICON:
			_listCIcon(o);
			break;

		case G_SWBUTTON:
			_listSWButton(o);
			break;

		case G_POPUP:
			_listPopup(o);
			break;
		}
	}

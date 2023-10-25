//
//  shellCfg.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/24/23.
//
#include <ctype.h>
#include <string.h>

#include "debug.h"
#include "gem.h"
#include "shellCfg.h"
#include "stringUtils.h"

#define WIN_DX	(8)
#define WIN_DY	(16)

typedef enum
	{
	FIRST_DIGIT = 0,
	SECOND_DIGIT
	} IconVariant;

/*****************************************************************************\
|* Forward references
\*****************************************************************************/
static int _parseSerial(ND_INFO *info, const char *data);
static int _parsePrinter(ND_INFO *info, const char *data);
static int _parseMisc(ND_INFO *info, const char *data);
static int _parseIconVec(vec_icon_t *vec, const char *data, IconVariant variant);
static int _parseIcon(ND_ICONSPEC *spec, const char *data, IconVariant variant);
static int _parseDriveVec(vec_drive_t *info, const char *data);
static int _parseDrive(ND_DRIVE *drive, const char *data);
static int _parseDisplayInfo(ND_INFO *info, const char *data);
static int _parseExecIcon(vec_gicon_t *vec, char *data);
static int _parseShortcuts(ND_INFO *info, char *data);
static int _parseWindowStyle(ND_INFO *info, char *data);
static int _parseCharset(ND_INFO *info, char *data);
static int _parseLinks(vec_link_t *vec, char *data);
static int _parseWindows(vec_win_t *vec, char *data);

#define CLEAN(vector, type) 												\
	if (vector.data != NULL) 												\
		{ 																	\
		int i; 																\
		type *entry; 														\
		vec_foreach(&(vector), entry, i) 									\
			free(entry); 													\
		vec_deinit(&(vector));												\
		vector.data = NULL;													\
		vector.length = vector.capacity = 0;								\
		} 																	\
	else 																	\
		vec_init(&(vector))
		

/*****************************************************************************\
|* Zero out an info structure
\*****************************************************************************/
void _gemInfZero(ND_INFO *info)
	{
	memset(info, 0, sizeof(ND_INFO));
	}

/*****************************************************************************\
|* Initialise an info structure so it's ready to use
\*****************************************************************************/
void _gemInfInit(ND_INFO *info)
	{
	/*************************************************************************\
	|* Accessories
	\*************************************************************************/
	CLEAN(info->accs, ND_ICONSPEC);
	
	/*************************************************************************\
	|* Cartridges
	\*************************************************************************/
	CLEAN(info->carts, ND_DRIVE);
	
	/*************************************************************************\
	|* Folders
	\*************************************************************************/
	CLEAN(info->folders, ND_ICONSPEC);
	
	/*************************************************************************\
	|* Commands
	\*************************************************************************/
	CLEAN(info->cmds, ND_ICONSPEC);
	
	/*************************************************************************\
	|* Apps
	\*************************************************************************/
	CLEAN(info->apps, ND_GEM_ICONSPEC);
	
	/*************************************************************************\
	|* Files
	\*************************************************************************/
	CLEAN(info->files, ND_ICONSPEC);
	
	/*************************************************************************\
	|* Shortcuts
	\*************************************************************************/
	if (info->shortcuts.data != NULL)
		vec_deinit(&(info->shortcuts));
	else
		vec_init(&(info->shortcuts));
	
	/*************************************************************************\
	|* Drives
	\*************************************************************************/
	CLEAN(info->drives, ND_DRIVE);
	
	/*************************************************************************\
	|* Commands that take parameters
	\*************************************************************************/
	CLEAN(info->cmdParams, ND_GEM_ICONSPEC);
	
	/*************************************************************************\
	|* Folder links on the desktop
	\*************************************************************************/
	CLEAN(info->fldrLinks, ND_LINK);
	
	/*************************************************************************\
	|* Windows
	\*************************************************************************/
	CLEAN(info->windows, ND_WINDOW);
	
	/*************************************************************************\
	|* File links on the desktop
	\*************************************************************************/
	CLEAN(info->fileLinks, ND_LINK);
	
	/*************************************************************************\
	|* Apps that take params
	\*************************************************************************/
	CLEAN(info->appParams, ND_GEM_ICONSPEC);
	}

/*****************************************************************************\
|* Parse a text representation into a structure representation
\*****************************************************************************/
int _gemInfReadData(const char *inf, ND_INFO* info)
	{
	int errors 	= 0;
	
	/*************************************************************************\
	|* Check to see if the info pointer and inf data are valid
	\*************************************************************************/
	if ((inf != NULL) && (info != NULL))
		{
		/*********************************************************************\
		|* Clear out the info structure so it's ready to use.
		\*********************************************************************/
		_gemInfInit(info);

		/*********************************************************************\
		|* Tokenise the strings into a reverse-order list
		\*********************************************************************/
		vec_str_t items;
		vec_init(&items);

		char *copy 	= strdup(inf);
		char *entry = NULL;
		
		for (entry = strtok(copy, "#"); entry; entry = strtok(NULL, "#"))
			vec_push(&items, entry);
		vec_reverse(&items);
		
		/*********************************************************************\
		|* Run over all the tokens, parsing in-order
		\*********************************************************************/
		int i;
		vec_foreach(&items, entry, i)
			{
			printf("* %s", entry);
			switch (entry[0])
				{
				case 'a':
					errors += _parseSerial(info, entry+1);
					break;
				case 'b':
					errors += _parsePrinter(info, entry+1);
					break;
				case 'c':
					errors += _parseMisc(info, entry+1);
					break;
				case 'd':
					break;
				case 'A':
					errors += _parseIconVec(&(info->accs), entry+1, FIRST_DIGIT);
					break;
				case 'C':
					errors += _parseDriveVec(&(info->carts), entry+1);
					break;
				case 'D':
					errors += _parseIconVec(&(info->folders), entry+1, SECOND_DIGIT);
					break;
				case 'E':
					errors += _parseDisplayInfo(info, entry+1);
					break;
				case 'F':
					errors += _parseIconVec(&(info->cmds), entry+1, FIRST_DIGIT);
					break;
				case 'G':
					errors += _parseExecIcon(&(info->apps), entry+1);
					break;
				case 'I':
					errors += _parseIconVec(&(info->files), entry+1, SECOND_DIGIT);
					break;
				case 'K':
					errors += _parseShortcuts(info, entry+1);
					break;
				case 'M':
					errors += _parseDriveVec(&(info->drives), entry+1);
					break;
				case 'N':
					errors += _parseIcon(&(info->catchAll), entry+1, SECOND_DIGIT);
					break;
				case 'O':
					errors += _parseDrive(&(info->printer), entry+1);
					break;
				case 'P':
					errors += _parseExecIcon(&(info->cmdParams), entry+1);
					break;
				case 'Q':
					errors += _parseWindowStyle(info, entry+1);
					break;
				case 'S':
					errors += _parseCharset(info, entry+1);
					break;
				case 'T':
					errors += _parseDrive(&(info->trash), entry+1);
					break;
				case 'V':
					errors += _parseLinks(&(info->fldrLinks), entry+1);
					break;
				case 'W':
					errors += _parseWindows(&(info->windows), entry+1);
					break;
				case 'X':
					errors += _parseLinks(&(info->fileLinks), entry+1);
					break;
				case 'Y':
					errors += _parseExecIcon(&(info->appParams), entry+1);
					break;
				}
			}
		free(copy);
		}
	
	return errors;
	}

/*****************************************************************************\
|* Parse out the serial comms data
\*****************************************************************************/
static int _parseSerial(ND_INFO *info, const char *data)
	{
	int error	= 1;
	uint8_t stop, handshake, bits, parity, baud, duplex;
	
	int tokens = sscanf(data, "%c%c%c%c%c%c", &duplex, &baud, &parity, &bits,
											  &handshake, &stop);
	if (tokens == 6)
		{
		error 					= 0;;
		info->serial.stopbit 	= (stop - '0');
		info->serial.protocol	= (handshake - '0');
		info->serial.bits		= (bits - '0');
		info->serial.parity		= (parity - '0');
		info->serial.baud		= (baud - '0');
		info->serial.duplex		= (duplex - '0');
		}
	return error;
	}

/*****************************************************************************\
|* Parse out the printer data
\*****************************************************************************/
static int _parsePrinter(ND_INFO *info, const char *data)
	{
	int error	= 1;
	uint8_t paper, port, quality, ppl, colour, type;
	
	int tokens = sscanf(data, "%c%c%c%c%c%c", &type, &colour, &ppl, &quality,
											  &port, &paper);
	if (tokens == 6)
		{
		error 					= 0;;
		info->printcfg.type 	= (type - '0');
		info->printcfg.colour	= (colour - '0');
		info->printcfg.ppl		= (ppl - '0');
		info->printcfg.quality	= (quality - '0');
		info->printcfg.port		= (port - '0');
		info->printcfg.paper	= (paper - '0');
		}
	return error;
	}

/*****************************************************************************\
|* Parse out the colours and mouse/keyboard settings
\*****************************************************************************/
static int _parseMisc(ND_INFO *info, const char *data)
	{
	int error	= 1;
	ND_RGB rgb;
	
	error += 16;
	for (int i=0; i<16; i++)
		{
		if (sscanf(data, "%c%c%c", &(rgb.r), &(rgb.g), &(rgb.b)) == 3)
			{
			info->misc.rgb[i] = rgb;
			error --;
			}
		else
			break;
		data += 3;
		}
	
	if (error == 1)
		{
		uint8_t bell, keyClk, dblClk;
		uint32_t keyRpt, rptDly;
		uint8_t tokens = sscanf(data,
								"%c%c%c%2x%2x",
								&dblClk, &keyClk, &bell, &rptDly, &keyRpt);
		if (tokens == 5)
			{
			error 					= 0;;
			info->misc.keyRepeat 	= (keyRpt - '0');
			info->misc.repeatDelay	= (rptDly - '0');
			info->misc.bell			= (bell - '0');
			info->misc.keyclick		= (keyClk - '0');
			info->misc.doubleClick	= (dblClk - '0');
			}
		}
	return error;
	}

/*****************************************************************************\
|* Parse out window icon data
\*****************************************************************************/
static int _parseIconVec(vec_icon_t *info,
						 const char *data,
						 IconVariant variant)
	{
	ND_ICONSPEC *spec = (ND_ICONSPEC *) malloc(sizeof(ND_ICONSPEC));
	int error 		  = _parseIcon(spec, data, variant);
	if (!error)
		{
		if (vec_push(info, spec))
			WARN("Cannot add icon entry for %s", data);
		}
	
	return error;
	}

static int _parseIcon(ND_ICONSPEC *spec, const char *data, IconVariant variant)
	{
	int error = 1;
	
	const char *fmt = (variant == FIRST_DIGIT)
					? "%02x %*s %*s %s"
					: "%*s %02x %*s %*s %s";
					
	if (sscanf(data, fmt, &(spec->iconId), spec->spec) == 2)
		{
		error = 0;
		int len = (int) strlen(spec->spec);
		if (spec->spec[len-1] == '@')
			spec->spec[len-1] = '\0';
		}
	
	return error;
	}

/*****************************************************************************\
|* Parse out drive data
\*****************************************************************************/
static int _parseDriveVec(vec_drive_t *info, const char *data)
	{
	ND_DRIVE *drive = (ND_DRIVE *) malloc(sizeof(ND_DRIVE));
	int error		= _parseDrive(drive, data);
	if (!error)
		{
		if (vec_push(info, drive))
			WARN("Cannot add drive entry for %s", data);
		}
	
	return error;
	}
	
/*****************************************************************************\
|* Parse out drive data
\*****************************************************************************/
static int _parseDrive(ND_DRIVE *drive, const char *data)
	{
	int error = 1;
	int iconId;
	if (sscanf(data, "%x %x %x %*s %c %s",
				&(drive->x),
			 	&(drive->y),
			 	&(iconId),
			 	&(drive->driveId),
			 	drive->text) == 5)
		{
		error = 0;
		drive->iconId = iconId;
		int len = (int) strlen(drive->text);
		if (drive->text[len-1] == '@')
			drive->text[len-1] = '\0';
		}
	
	return error;
	}
	
/*****************************************************************************\
|* Parse out the display info data
\*****************************************************************************/
static int _parseDisplayInfo(ND_INFO *info, const char *data)
	{
	int error	= 1;
	int preset, blitter, flags, dbl, colours;
	
	int tokens = sscanf(data, "%x %x %*x %x %x %x",
						&preset, &blitter, &flags, &dbl, &colours);
	if ((tokens == 3) || (tokens == 5))
		{
		error 							= 0;;
		info->dpyInfo.presets 			= preset;
		info->dpyInfo.blitter			= ((blitter  >> 4)) > 0 ? 1 : 0;
		info->dpyInfo.resolution		= blitter & 0xF;
		info->dpyInfo.flags				= flags;
		if (tokens > 3)
			{
			info->dpyInfo.lineDouble	= dbl;
			info->dpyInfo.colours		= colours;
			}
		else
			{
			info->dpyInfo.lineDouble	= 0;
			info->dpyInfo.colours		= 0;
			}
		}
	return error;
	}


/*****************************************************************************\
|* Parse out window icon data
\*****************************************************************************/
static int _parseExecIcon(vec_gicon_t *info, char *data)
	{
	int error = 1;
	
	char *phrase = strtok(data, "@");
	if (phrase != NULL)
		{
		int flags;
		int iconId;
		
		ND_GEM_ICONSPEC *icon = (ND_GEM_ICONSPEC *) malloc(sizeof(ND_GEM_ICONSPEC));
		if (sscanf(data, "%x %*s %03x %s",
					&iconId,
					&flags,
					icon->path) == 3)
			{
			icon->iconId	= iconId & 0xFF;
			icon->flags 	= flags >> 8;
			icon->fnKey		= flags & 0xFF;
			icon->spec[0]	= '\0';
			icon->params[0]	= '\0';
			
			phrase = strtok(NULL, "@");
			if (phrase)
				strncpy(icon->spec, strTrim(phrase), 32);

			phrase = strtok(NULL, "@");
			if (phrase)
				{
				sscanf(phrase, "%x", &(icon->maxMem));
				error = 0;
				}
			}
		if (vec_push(info, icon))
			WARN("Cannot add exec-icon entry for %s", data);
		}
	
	return error;
	}

/*****************************************************************************\
|* Parse out shortcut data
\*****************************************************************************/
static int _parseShortcuts(ND_INFO *info, char *data)
	{
	int error	= 1;

	char *hex  	= strtok(data, " ");
	if (hex)
		{
		error = 0;
		while (hex)
			{
			if (hex[0] == '@')
				break;
			
			int key;
			sscanf(hex, "%x", &key);
			vec_push(&(info->shortcuts), (uint8_t)(key & 0xFF));
			
			hex = strtok(NULL, " ");
			}
		}
	return error;
	}

/*****************************************************************************\
|* Parse out window/desktop patterns and colours
\*****************************************************************************/
static int _parseWindowStyle(ND_INFO *info, char *data)
	{
	int error	= 1;
	
	int winCol, winPat, dskCol, dskPat;
	
	int tokens = sscanf(data, "%x %x %x %x", &dskPat, &dskCol, &winPat, &winCol);
	if (tokens == 4)
		{
		error 							= 0;
		info->winStyle.winColour 		= winCol;
		info->winStyle.desktopColour	= dskCol;
		info->winStyle.windowPattern	= winPat;
		info->winStyle.desktopPattern	= dskPat;
		}
		
	return error;
	}

/*****************************************************************************\
|* Parse out system font and size
\*****************************************************************************/
static int _parseCharset(ND_INFO *info, char *data)
	{
	int error	= 1;
	
	int pts, font;
	
	int tokens = sscanf(data, "%x %x", &font, &pts);
	if (tokens == 2)
		{
		error 					= 0;
		info->charset.points	= pts;
		info->charset.font		= font;
		}
		
	return error;
	}
	
/*****************************************************************************\
|* Parse out folder/file links on the desktop
\*****************************************************************************/
static int _parseLinks(vec_link_t *info, char *data)
	{
	int error = 1;

	ND_LINK *link = (ND_LINK *) malloc(sizeof(ND_LINK));

	int iconId;
	if (sscanf(data, "%x %x %x %*s %s %s",
				&(link->x),
			 	&(link->y),
			 	&(iconId),
			 	link->pathSpec,
			 	link->iconText) == 5)
		{
		error = 0;
		link->iconId = iconId & 0xFF;
		
		int len = (int) strlen(link->pathSpec);
		if (link->pathSpec[len-1] == '@')
			link->pathSpec[len-1] = '\0';
		len = (int) strlen(link->iconText);
		if (link->iconText[len-1] == '@')
			link->iconText[len-1] = '\0';
		}
	
	if (!error)
		{
		if (vec_push(info, link))
			WARN("Cannot add link entry for %s", data);
		}
	return error;
	}

/*****************************************************************************\
|* Parse out window size/positions
\*****************************************************************************/
static int _parseWindows(vec_win_t *vec, char *data)
	{
	int error = 1;

	ND_WINDOW *win = (ND_WINDOW *) malloc(sizeof(ND_WINDOW));
	if (sscanf(data, "%x %x %x %x %x %x %x %s",
				&(win->hs),
				&(win->vs),
				&(win->x),
			 	&(win->y),
			 	&(win->w),
			 	&(win->h),
			 	&(win->status),
			 	win->pathSpec) == 8)
		{
		error 		= 0;
		win->hs		= win->hs * WIN_DX;
		win->x		= win->x * WIN_DX;
		win->w		= win->w * WIN_DX;
		win->vs		= win->vs * WIN_DY;
		win->y		= win->y * WIN_DY;
		win->h		= win->h * WIN_DY;

		int len = (int) strlen(win->pathSpec);
		if (win->pathSpec[len-1] == '@')
			win->pathSpec[len-1] = '\0';
		}
		
	if (!error)
		{
		if (vec_push(vec, win))
			WARN("Cannot add window entry for %s", data);
		}
		
	return error;
	}

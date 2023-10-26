//
//  shell_parse.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/24/23.
//
#include <ctype.h>
#include <string.h>

#include "debug.h"
#include "gem.h"
#include "shell_conv.h"
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
static int _parseDrive(ND_DRIVE *drive, const char *data, int driveId);
static int _parseDisplayInfo(ND_INFO *info, const char *data);
static int _parseExecIcon(vec_gicon_t *vec, char *data);
static int _parseShortcuts(ND_INFO *info, char *data);
static int _parseWindowStyle(ND_INFO *info, char *data);
static int _parseCharset(ND_INFO *info, char *data);
static int _parseLinks(vec_link_t *vec, char *data);
static int _parseWindows(vec_win_t *vec, char *data);

static void _writeIconSpec(vec_icon_t icons, char type, vec_char_t *result);
static void _writeGemIconSpec(vec_gicon_t icons, char type, vec_char_t *result);


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
					errors += _parseDrive(&(info->printer), entry+1, 0);
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
					errors += _parseDrive(&(info->trash), entry+1, 0);
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
|* Parse a structure representation into a text representation. Returns an
|* allocated string that must be free'd
\*****************************************************************************/
int _gemInfWriteData(ND_INFO* info, char **inf)
	{
	vec_char_t result;		// Expandable string for results
	char line[4096];		// Ought to be plenty for a single line
	
	// Initialise the stores
	vec_init(&result);
	line[0] = 0;
	
	/*************************************************************************\
	|* Handle the serial token
	\*************************************************************************/
	sprintf(line, "#a%c%c%c%c%c%c\n", info->serial.duplex + '0',
									  info->serial.baud + '0',
									  info->serial.parity + '0',
									  info->serial.bits + '0',
									  info->serial.protocol + '0',
									  info->serial.stopbit + '0');
	vec_pusharr(&result, line, 9);
	
	/*************************************************************************\
	|* Handle the printer token
	\*************************************************************************/
	sprintf(line, "#b%c%c%c%c%c%c\n", info->printcfg.type + '0',
									  info->printcfg.colour + '0',
									  info->printcfg.ppl + '0',
									  info->printcfg.quality + '0',
									  info->printcfg.port + '0',
									  info->printcfg.paper + '0');
	vec_pusharr(&result, line, 9);
	
	
	/*************************************************************************\
	|* Handle the colours token
	\*************************************************************************/
	vec_pusharr(&result, "#c", 2);
	for (int i=0; i<16; i++)
		{
		sprintf(line, "%1d%1d%1d", info->misc.rgb[i].r,
								   info->misc.rgb[i].g,
								   info->misc.rgb[i].b);
		vec_pusharr(&result, line, 3);
		}
	sprintf(line, "%1d%1d%1d%02x%02x\n", info->misc.doubleClick,
									     info->misc.keyclick,
									     info->misc.bell,
									     info->misc.repeatDelay,
									     info->misc.keyRepeat);
	vec_pusharr(&result, line, 8);

	/*************************************************************************\
	|* Handle the 'waste a bunch of space' token
	\*************************************************************************/
	vec_pusharr(&result, "#d", 2);
	for (int i=result.length; i<127; i++)
		vec_push(&result, ' ');
	vec_push(&result, '\n');
	
	/*************************************************************************\
	|* Do the "K" token, keyboard shortcuts
	\*************************************************************************/
	vec_pusharr(&result, "#K", 2);
	for (int i=0; i<info->shortcuts.length; i++)
		{
		sprintf(line, " %02x", info->shortcuts.data[i]);
		vec_pusharr(&result, line, 3);
		}
	vec_pusharr(&result, " @\n", 3);
	
	/*************************************************************************\
	|* Do the "E" token, display configuration
	\*************************************************************************/
	sprintf(line, "#E %02x %01x%01x 00 %02x %02x %02x 00 00 00 00\n",
			info->dpyInfo.presets,
			info->dpyInfo.blitter,
			info->dpyInfo.resolution,
			info->dpyInfo.flags,
			info->dpyInfo.lineDouble,
			info->dpyInfo.colours);
	vec_pusharr(&result, line, 33);

	/*************************************************************************\
	|* Do the "Q" token, desktop/window fill-pattern, colour
	\*************************************************************************/
	sprintf(line, "#Q %02x %02x %02x %02x\n",
			info->winStyle.desktopPattern,
			info->winStyle.desktopColour,
			info->winStyle.windowPattern,
			info->winStyle.winColour);
	vec_pusharr(&result, line, 15);

	/*************************************************************************\
	|* Do the "C" token, cartridge.
	\*************************************************************************/
	for (int i=0; i<info->carts.length; i++)
		{
		ND_DRIVE *cart = info->carts.data[i];
		
		sprintf(line, "#C %02x %02x %02x FF %c %s@ @ \n",
				cart->x,
				cart->y,
				cart->iconId,
				cart->driveId,
				cart->text);
		vec_pusharr(&result, line, (int)strlen(line));
		}

	/*************************************************************************\
	|* Do the "M" token, "drives"
	\*************************************************************************/
	for (int i=0; i<info->drives.length; i++)
		{
		ND_DRIVE *drive = info->drives.data[i];
		
		sprintf(line, "#M %02x %02x %02x FF %c %s@ @ \n",
				drive->x,
				drive->y,
				drive->iconId,
				drive->driveId,
				drive->text);
		vec_pusharr(&result, line, (int)strlen(line));
		}

	/*************************************************************************\
	|* Do the "T" token, trash icon
	\*************************************************************************/
	sprintf(line, "#T %02x %02x %02x FF   %s@ @ \n",
			info->trash.x,
			info->trash.y,
			info->trash.iconId,
			info->trash.text);
	vec_pusharr(&result, line, (int)strlen(line));

	/*************************************************************************\
	|* Do the "O" token, printer icon
	\*************************************************************************/
	if (strlen(info->printer.text) > 0)
		{
		sprintf(line, "#O %02x %02x %02x FF   %s@ @ \n",
				info->printer.x,
				info->printer.y,
				info->printer.iconId,
				info->printer.text);
		vec_pusharr(&result, line, (int)strlen(line));
		}

	/*************************************************************************\
	|* Do the "W" token, window locations etc.
	\*************************************************************************/
	for (int i=0; i<info->windows.length; i++)
		{
		ND_WINDOW *win = info->windows.data[i];
		
		sprintf(line, "#W %02x %02x %02x %02x %02x %02x %02x %s@\n",
				win->hs/WIN_DX,
				win->vs/WIN_DY,
				win->x/WIN_DX,
				win->y/WIN_DY,
				win->w/WIN_DX,
				win->h/WIN_DY,
				win->status,
				win->pathSpec);
		vec_pusharr(&result, line, (int)strlen(line));
		}

	/*************************************************************************\
	|* Do the "N" token, default file icon
	\*************************************************************************/
	sprintf(line, "#N FF %02x 000 @ *.*@ @ \n", info->catchAll.iconId);
	vec_pusharr(&result, line, 23);

	/*************************************************************************\
	|* Do the "D" token, folder-icon specs
	\*************************************************************************/
	_writeIconSpec(info->folders, 'D', &result);

	/*************************************************************************\
	|* Do the "G" token, gem-applications, wildcard and registered-apps
	\*************************************************************************/
	_writeGemIconSpec(info->apps, 'G', &result);

	/*************************************************************************\
	|* Do the "Y" token, gem-takes-params, wildcard and registered-apps
	\*************************************************************************/
	_writeGemIconSpec(info->appParams, 'Y', &result);

	/*************************************************************************\
	|* Do the "P" token, tos-takes-params, wildcard and registered-apps
	\*************************************************************************/
	_writeGemIconSpec(info->cmdParams, 'P', &result);
	
	/*************************************************************************\
	|* Do the "F" token, tos-applications, wildcard and registered-apps
	\*************************************************************************/
	_writeIconSpec(info->folders, 'F', &result);

	/*************************************************************************\
	|* Do the "A" token, accessories, wildcard and registered-apps
	\*************************************************************************/
	_writeIconSpec(info->accs, 'A', &result);

	/*************************************************************************\
	|* Do the "I" token, data-files, wildcard and registered-apps
	\*************************************************************************/
	_writeIconSpec(info->files, 'I', &result);


	/*************************************************************************\
	|* Convert the vector to a char ptr and return it
	\*************************************************************************/
	char * text = (char *) malloc(result.length+1);
	memcpy(text, result.data, result.length+1);
	if (inf)
		*inf = text;
		
	vec_deinit(&result);
	return 1;
	}

/*****************************************************************************\
|* Helpers for sorting
\*****************************************************************************/
typedef struct
	{
	int idx;
	int priority;
	} IconOrder;

static int sortOrder(const void *o1, const void *o2)
	{
	IconOrder *one = (IconOrder *)o1;
	IconOrder *two = (IconOrder *)o2;
	
	return (two->priority > one->priority) ? 1
		 : (two->priority < one->priority) ? -1
		 : 0;
	}

	
/*****************************************************************************\
|* Write out a vector of ND_ICONSPEC, ordering by the number of * chars in the
|* filespec
\*****************************************************************************/
static void _writeIconSpec(vec_icon_t icons, char type, vec_char_t *result)
	{
	/*************************************************************************\
	|* Set up an indirection array to easily sort based on the number of '*'
	|* characters in the spec
	\*************************************************************************/
	IconOrder order[icons.length];
	for (int i=0; i<icons.length; i++)
		{
		int stars	= 0;
		char *spec	= icons.data[i]->spec;
		while (*spec)
			{
			if (*spec == '*')
				stars ++;
			spec++;
			}
		order[i].idx 		= i;
		order[i].priority	= stars;
		}
	
	/*************************************************************************\
	|* sort...
	\*************************************************************************/
	qsort(order, icons.length, sizeof(IconOrder), sortOrder);
	
	/*************************************************************************\
	|* and output
	\*************************************************************************/
	char line[4096];
	
	for (int i=0; i<icons.length; i++)
		{
		ND_ICONSPEC *icon 	= icons.data[order[i].idx];
		int len				= (int) strlen(icon->spec);
		int isCatchall		= (strncmp(icon->spec, "*.*", 3) == 0);
		
		if ((type == 'A') || (type == 'F'))
			{
			const char *spec = (type == 'A') ? ("*.ACC") : ("*.TOS");
			sprintf(line, "#%c %02x 04 000 @ %s@ @ @ \n",
						type,
						icon->iconId,
						spec);
			}
		else if ((len == 3) && isCatchall)
			sprintf(line, "#%c FF %02x 000 @ *.*@ @ \n", type, icon->iconId);
		else
			sprintf(line, "#%c %02x %02x 000 @ %s@ @ \n",
					type,
					icon->iconId,
					icon->iconId,
					icon->spec);
					
		vec_pusharr(result, line, (int)strlen(line));
		}
	}
	
/*****************************************************************************\
|* Write out a vector of ND_GEM_ICONSPEC, ordering by how specific the
|* filespec is
\*****************************************************************************/
static void _writeGemIconSpec(vec_gicon_t icons, char type, vec_char_t *result)
	{
	/*************************************************************************\
	|* Set up an indirection array to easily sort based on the specificity of
	|* the spec
	\*************************************************************************/
	IconOrder order[icons.length];
	for (int i=0; i<icons.length; i++)
		{
		ND_GEM_ICONSPEC *current 	= icons.data[i];
		order[i].idx 				= i;

		if (strlen(current->path) == 0)
			order[i].priority	= 10000;
		else
			{
			int stars	= 0;
			char *spec	= current->spec;
			while (*spec)
				{
				if (*spec == '*')
					stars ++;
				spec++;
				}
			order[i].priority	= stars;
			}
		}
	
	/*************************************************************************\
	|* sort...
	\*************************************************************************/
	qsort(order, icons.length, sizeof(IconOrder), sortOrder);
	
	/*************************************************************************\
	|* and output
	\*************************************************************************/
	char line[4096];
	
	for (int i=0; i<icons.length; i++)
		{
		ND_GEM_ICONSPEC *icon 	= icons.data[order[i].idx];
		
		if (strlen(icon->path) == 0)
			sprintf(line, "#%c %02x FF 000 @ %s@ @ @ \n",
					type,
					icon->iconId,
					icon->spec);
		else
			sprintf(line, "#%c %02x 04 %01x%02x %s@ %s@ @ \n",
					type,
					icon->iconId,
					icon->flags,
					icon->fnKey,
					icon->path,
					icon->spec);
					
		vec_pusharr(result, line, (int)strlen(line));
		}
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
			info->misc.keyRepeat 	= keyRpt;
			info->misc.repeatDelay	= rptDly;
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
	int error		= _parseDrive(drive, data, 1);
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
static int _parseDrive(ND_DRIVE *drive, const char *data, int driveId)
	{
	int error = 1;
	int iconId;
	int parsed = 0;
	if (driveId)
		parsed = (sscanf(data, "%x %x %x %*s %c %s",
							&(drive->x),
							&(drive->y),
							&(iconId),
							&(drive->driveId),
							drive->text) == 5);
	else
		parsed = (sscanf(data, "%x %x %x %*s %s",
							&(drive->x),
							&(drive->y),
							&(iconId),
							drive->text) == 4);
		
	if (parsed)
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
				{
				strncpy(icon->spec, strTrim(phrase), 32);
				if (strlen(icon->spec) == 0)
					{
					char buf[4096];
					strcpy(buf, icon->spec);
					strcpy(icon->spec, icon->path);
					strcpy(icon->path, buf);
					}
				}
				
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

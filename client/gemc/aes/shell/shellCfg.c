//
//  shellCfg.c
//  gemc
//
//  Created by ThrudTheBarbarian on 10/24/23.
//
#include <string.h>

#include "debug.h"
#include "gem.h"
#include "shellCfg.h"

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
static int _parseIcon(vec_icon_t *vec, const char *data, IconVariant variant);
static int _parseDrive(vec_drive_t *vec, const char *data);
static int _parseDisplayInfo(ND_INFO *info, const char *data);
static int _parseExecIcon(vec_gicon_t *vec, const char *data);

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
	CLEAN(info->fldrLinks, ND_LINK_FLDR);
	
	/*************************************************************************\
	|* Windows
	\*************************************************************************/
	CLEAN(info->windows, ND_WINDOW);
	
	/*************************************************************************\
	|* File links on the desktop
	\*************************************************************************/
	CLEAN(info->fileLinks, ND_LINK_FLDR);
	
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
		|* Run over all the tokens, parsing in-order
		\*********************************************************************/
		char *copy 	= strdup(inf);
		char *entry = NULL;
		
		for (entry = strtok(copy, "#"); entry; entry = strtok(NULL, "#"))
			{
			printf("%s", entry);
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
					errors += _parseIcon(&(info->accs), entry+1, FIRST_DIGIT);
					break;
				case 'C':
					errors += _parseDrive(&(info->carts), entry+1);
					break;
				case 'D':
					errors += _parseIcon(&(info->folders), entry+1, SECOND_DIGIT);
					break;
				case 'E':
					errors += _parseDisplayInfo(info, entry+1);
					break;
				case 'F':
					errors += _parseIcon(&(info->cmds), entry+1, FIRST_DIGIT);
					break;
				case 'G':
					errors += _parseExecIcon(&(info->apps), entry+1);
					break;
				case 'I':
					errors += _parseIcon(&(info->files), entry+1, SECOND_DIGIT);
					break;
				case 'K':
					break;
				case 'M':
					errors += _parseDrive(&(info->drives), entry+1);
					break;
				case 'N':
					break;
				case 'O':
					break;
				case 'P':
					break;
				case 'Q':
					break;
				case 'S':
					break;
				case 'T':
					break;
				case 'V':
					break;
				case 'W':
					break;
				case 'X':
					break;
				case 'Y':
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
static int _parseIcon(vec_icon_t *info, const char *data, IconVariant variant)
	{
	int error = 1;
	
	const char *fmt = (variant == FIRST_DIGIT)
					? "%02x %*s %*s %s"
					: "%*s %02x %*s %*s %s";
					
	ND_ICONSPEC *spec = (ND_ICONSPEC *) malloc(sizeof(ND_ICONSPEC));
	if (sscanf(data, fmt, &(spec->iconId), spec->spec) == 2)
		{
		error = 0;
		int len = (int) strlen(spec->spec);
		if (spec->spec[len-1] == '@')
			spec->spec[len-1] = '\0';
		
		if (vec_push(info, spec))
			WARN("Cannot add icon entry for %s", data);
		}
	
	return error;
	}

/*****************************************************************************\
|* Parse out drive data
\*****************************************************************************/
static int _parseDrive(vec_drive_t *info, const char *data)
	{
	int error = 1;
	
	const char *fmt = "%x %x %x %*s %c %s";
					
	ND_DRIVE *drive = (ND_DRIVE *) malloc(sizeof(ND_DRIVE));
	if (sscanf(data, fmt, &(drive->x),
						  &(drive->y),
						  &(drive->iconId),
						  &(drive->driveId),
						  drive->text) == 5)
		{
		error = 0;
		int len = (int) strlen(drive->text);
		if (drive->text[len-1] == '@')
			drive->text[len-1] = '\0';
		
		if (vec_push(info, drive))
			WARN("Cannot add drive entry for %s", data);
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
static int _parseExecIcon(vec_gicon_t *info, const char *data)
	{
	int error = 1;
	
	int flags;
	
	const char *fmt = "%x %*s %03x";
					
	ND_ICONSPEC *spec = (ND_ICONSPEC *) malloc(sizeof(ND_ICONSPEC));
	if (sscanf(data, fmt, &(spec->iconId), spec->spec) == 2)
		{
		error = 0;
		int len = (int) strlen(spec->spec);
		if (spec->spec[len-1] == '@')
			spec->spec[len-1] = '\0';
		
		if (vec_push(info, spec))
			WARN("Cannot add icon entry for %s", data);
		}
	
	return error;
	}

//
//  shellCfg.c
//  gemc
//
//  Created by Simon Gornall on 10/24/23.
//
#include <string.h>

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

/*****************************************************************************\
|* Zero out an info structure
\*****************************************************************************/
void _gemInfZero(ND_INFO *info)
	{
	memset(info, 0, sizeof(ND_INFO));
	}

//FIXME: Need to handle memory properly for each type

/*****************************************************************************\
|* Initialise an info structure so it's ready to use
\*****************************************************************************/
void _gemInfInit(ND_INFO *info)
	{
	/*************************************************************************\
	|* Accessories
	\*************************************************************************/
	if (info->accs.data != NULL)
		{
		int i;
		ND_ICONSPEC *entry;
		vec_foreach(&(info->accs), entry, i)
			free(entry);
			
		vec_deinit(&(info->accs));
		}
	else
		vec_init(&(info->accs));
	
	/*************************************************************************\
	|* Cartridges
	\*************************************************************************/
	if (info->carts.data != NULL)
		vec_deinit(&(info->carts));
	else
		vec_init(&(info->carts));
	
	/*************************************************************************\
	|* Folders
	\*************************************************************************/
	if (info->folders.data != NULL)
		{
		int i;
		ND_ICONSPEC *entry;
		vec_foreach(&(info->folders), entry, i)
			free(entry);
			
		vec_deinit(&(info->folders));
		}
	else
		vec_init(&(info->folders));
	
	/*************************************************************************\
	|* Commands
	\*************************************************************************/
	if (info->cmds.data != NULL)
		{
		int i;
		ND_ICONSPEC *entry;
		vec_foreach(&(info->cmds), entry, i)
			free(entry);
			
		vec_deinit(&(info->cmds));
		}
	else
		vec_init(&(info->cmds));
	
	/*************************************************************************\
	|* Apps
	\*************************************************************************/
	if (info->apps.data != NULL)
		vec_deinit(&(info->apps));
	else
		vec_init(&(info->apps));
	
	/*************************************************************************\
	|* Files
	\*************************************************************************/
	if (info->files.data != NULL)
		{
		int i;
		ND_ICONSPEC *entry;
		vec_foreach(&(info->files), entry, i)
			free(entry);
			
		vec_deinit(&(info->files));
		}
	else
		vec_init(&(info->files));
	
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
	if (info->drives.data != NULL)
		vec_deinit(&(info->drives));
	else
		vec_init(&(info->drives));
	
	/*************************************************************************\
	|* Commands that take parameters
	\*************************************************************************/
	if (info->cmdParams.data != NULL)
		vec_deinit(&(info->cmdParams));
	else
		vec_init(&(info->cmdParams));
	
	/*************************************************************************\
	|* Folder links on the desktop
	\*************************************************************************/
	if (info->fldrLinks.data != NULL)
		vec_deinit(&(info->fldrLinks));
	else
		vec_init(&(info->fldrLinks));
	
	/*************************************************************************\
	|* Windows
	\*************************************************************************/
	if (info->windows.data != NULL)
		vec_deinit(&(info->windows));
	else
		vec_init(&(info->windows));
	
	/*************************************************************************\
	|* File links on the desktop
	\*************************************************************************/
	if (info->fileLinks.data != NULL)
		vec_deinit(&(info->fileLinks));
	else
		vec_init(&(info->fileLinks));
	
	/*************************************************************************\
	|* Apps that take params
	\*************************************************************************/
	if (info->appParams.data != NULL)
		vec_deinit(&(info->appParams));
	else
		vec_init(&(info->appParams));
	}


/*****************************************************************************\
|* Destroy an Inf structure once we've used it up
\*****************************************************************************/
void _gemInfDeInit(ND_INFO *info)
	{
	int i;
	ND_ICONSPEC *iconspec;
	
	vec_foreach(&(info->accs), iconspec, i)
		free(iconspec);
	vec_deinit(&(info->accs));
	vec_deinit(&(info->carts));
	vec_foreach(&(info->folders), iconspec, i)
		free(iconspec);
	vec_deinit(&(info->folders));
	vec_foreach(&(info->cmds), iconspec, i)
		free(iconspec);
	vec_deinit(&(info->cmds));
	vec_deinit(&(info->apps));
	vec_foreach(&(info->files), iconspec, i)
		free(iconspec);
	vec_deinit(&(info->files));
	vec_deinit(&(info->shortcuts));
	vec_deinit(&(info->drives));
	vec_deinit(&(info->cmdParams));
	vec_deinit(&(info->fldrLinks));
	vec_deinit(&(info->windows));
	vec_deinit(&(info->fileLinks));
	vec_deinit(&(info->appParams));
	_gemInfZero(info);
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
					break;
				case 'D':
					errors += _parseIcon(&(info->folders), entry+1, SECOND_DIGIT);
					break;
				case 'E':
					break;
				case 'F':
					errors += _parseIcon(&(info->cmds), entry+1, FIRST_DIGIT);
					break;
				case 'G':
					break;
				case 'I':
					errors += _parseIcon(&(info->files), entry+1, SECOND_DIGIT);
					break;
				case 'K':
					break;
				case 'M':
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
		
		int ok = vec_push(info, spec);
		printf("ok=%d\n", ok);
		}
	
	return error;
	}

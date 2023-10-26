//
//  shellCfg.h
//  gemc
//
//  Created by ThrudTheBarbarian on 10/24/23.
//

#ifndef shellCfg_h
#define shellCfg_h

#include <limits.h>

#include "vec.h"

/*****************************************************************************\
|*	 #a000000
|*	   ||||||
|*	   |||||+--- Bit 8:		 	0: stop bit
|*	   ||||| 					1: no stop bit
|*	   |||||
|*	   ||||+---- Protocol: 		0: none
|*	   |||| 					1: Xon/Xoff
|*	   |||| 					2: Rts/Cts
|*	   |||| 					3: Xon/Xoff & Rts/Cts
|*	   ||||
|*	   |||+----- Word length: 	0:8
|*	   ||| 						1:7
|*	   ||| 						2:6
|*	   ||| 						3:5
|*	   |||
|*	   ||+------ Parity: 		0: none
|*	   || 						1: odd
|*	   || 						: even
|*	   ||
|*	   |+------- Baud: 			0: 9600 	5: 3600 	(:): 200 	(?): 50
|*	   | 						1: 4800 	6: 2400 	(;): 150
|*	   | 						2: 1200 	7: 2000 	(<): 134
|*	   | 						3: 300 		8: 1800 	(=): 110
|*	   | 						4: 19200 	9: 600 		(>): 75
|*	   |
|*	   +-------- Duplex: 		0: full
|*	                        	1: half
\*****************************************************************************/

typedef enum
	{
	ND_RS232_STOP_BIT 		= 0,
	ND_RS232_NO_STOP_BIT
	} ND_RS232_STOPBIT;

typedef enum
	{
	ND_RS232_NO_HANDSHAKE	= 0,
	ND_RS232_XON_XOFF		= 1,
	ND_RS232_RTS_CTS		= 2,
	ND_RS232_BOTH			= 3
	} ND_RS232_PROTOCOL;

typedef enum
	{
	ND_RS232_BITS_8	 		= 0,
	ND_RS232_BITS_7,
	ND_RS232_BITS_6,
	ND_RS232_BITS_5
	} ND_RS232_WORD_LEN;

typedef enum
	{
	ND_RS232_PARITY_NONE 	= 0,
	ND_RS232_PARITY_ODD,
	ND_RS232_PARITY_EVEN
	} ND_RS232_PARITY;

typedef enum
	{
	ND_RS232_BAUD_9600		= 0,
	ND_RS232_BAUD_4800,
	ND_RS232_BAUD_1200,
	ND_RS232_BAUD_300,
	ND_RS232_BAUD_19200,
	ND_RS232_BAUD_3600,
	ND_RS232_BAUD_2400,
	ND_RS232_BAUD_2000,
	ND_RS232_BAUD_1800,
	ND_RS232_BAUD_600,
	ND_RS232_BAUD_200,
	ND_RS232_BAUD_150,
	ND_RS232_BAUD_134,
	ND_RS232_BAUD_110,
	ND_RS232_BAUD_75,
	ND_RS232_BAUD_50
	} ND_RS232_BAUD;

typedef enum
	{
	ND_RS232_DUPLEX_FULL = 0,
	ND_RS232_DUPLEX_HALF
	} ND_RS232_DUPLEX;
	
typedef struct
	{
	ND_RS232_STOPBIT 	stopbit;		// Use a stop-bit or not
	ND_RS232_PROTOCOL 	protocol;		// Handshake protocol
	ND_RS232_WORD_LEN 	bits;			// Bits in a word
	ND_RS232_PARITY 	parity;			// None, odd or even
	ND_RS232_BAUD     	baud;			// Transfer speed
	ND_RS232_DUPLEX 	duplex;			// Half or full duplex
	} ND_RS232;

/*****************************************************************************\
|*	#b000000
|*	   ||||||
|*	   |||||+--- Paper Type: 	0: Single
|*	   ||||| 					1: Endless
|*	   |||||
|*	   ||||+---- Printer port: 	0: modem
|*	   |||| 					1: Printer
|*	   ||||
|*	   |||+----- Quality: 		0: Maximum
|*	   ||| 						1: Draft
|*	   |||
|*	   ||+------ Points/Line: 	0: 960
|*	   || 						1:1280
|*	   ||
|*	   |+------- Color: 		0: Color
|*	   | 						1: B/W
|*	   |
|*	   +-------- Printer type: 	0: Type wheel
|*	                           	1: Matrix
\*****************************************************************************/

typedef enum
	{
	ND_PRINT_PAPER_SINGLE		= 0,
	ND_PRINT_PAPER_ENDLESS
	} ND_PRINTER_PAPER;

typedef enum
	{
	ND_PRINT_PORT_SERIAL		= 0,
	ND_PRINT_PORT_PARALLEL
	} ND_PRINTER_PORT;

typedef enum
	{
	ND_PRINT_QUALITY_HIGH		= 0,
	ND_PRINT_QUALITY_DRAFT
	} ND_PRINTER_QUALITY;

typedef enum
	{
	ND_PRINT_PPL_960			= 0,
	ND_PRINT_PPL_1200
	} ND_PRINTER_PPL;

typedef enum
	{
	ND_PRINT_COLOUR				= 0,
	ND_PRINT_BW
	} ND_PRINTER_COLOUR;

typedef enum
	{
	ND_PRINT_TYPE_WHEEL			= 0,
	ND_PRINT_TYPE_MATRIX		= 1,
	ND_PRINT_TYPE_LASER			= 2
	} ND_PRINTER_TYPE;

typedef struct
	{
	ND_PRINTER_PAPER		paper;		// Single sheet or endless print
	ND_PRINTER_PORT			port;		// Where the printer is connected
	ND_PRINTER_QUALITY		quality;	// How much effort to put into printing
	ND_PRINTER_PPL			ppl;		// Points per line
	ND_PRINTER_COLOUR		colour;		// Is this a colour printer
	ND_PRINTER_TYPE			type;		// Laser, daisywheel, or dot-matrix
	} ND_PRINTER;


/*****************************************************************************\
|* #c777000...5507703111103
|*   | 			   ||||||||
|*   |- 16 groups -||||||++--- Key repeat rate
|*     	of 3 with	|||||
|*      color       |||++----- Key delay until repeat
|*      settings    |||
|*                  ||+------- Bell: 	0: off
|*                  || 					1: on
|*                  ||
|*                  |+-------- Click: 	0: off
|*                  | 					1: on
|*                  |
|*                  +--------- Double click rate
\*****************************************************************************/

typedef struct
	{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	} ND_RGB;

typedef enum
	{
	ND_BELL_OFF 			= 0,
	ND_BELL_ON
	} ND_BELL;

typedef enum
	{
	ND_KEYCLICK_OFF 		= 0,
	ND_KEYCLICK_ON
	} ND_KEYCLICK;
	
typedef struct
	{
	ND_RGB		rgb[16];				// The 16 indices of RGB, scaled 0..255
	uint8_t 	keyRepeat;				// Key repeat-rate, 0..3
	uint8_t 	repeatDelay;			// Delay before key repeats, 0..3
	ND_BELL		bell;					// Bell off or on
	ND_KEYCLICK	keyclick;				// Key-click off or on
	uint8_t		doubleClick;			// Mouse double-click rate, 0..3
	} ND_UI_MISC;

/*****************************************************************************\
|* #A_03_04_000_*.ACC@_@_@_
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | +---+--- Standard ending of an accessory
|*    || || | |
|*    || || +-+--------- always 000
|*    || ||
|*    || ++------------- always 04
|*    ||
|*    |+---------------- Object number (HEX) of the icon from the
|*                       the deskicon.rsc (or deskcicn.rsc)
\*****************************************************************************/

typedef struct
	{
	char 		spec[PATH_MAX];			// spec for match + \0. Maybe overkill
	uint8_t		iconId;					// Hex value of the icon in the RSC
	} ND_ICONSPEC;

typedef vec_t(ND_ICONSPEC*)	vec_icon_t;	// Vector of ND_ICONSPEC

/*****************************************************************************\
|* #C_00_02_07_FF_c_CARTRIDGE@_@_
|*    || || || || | |       |
|*    || || || || | +-------+--- Icon text for the drive icon
|*    || || || || |
|*    || || || || +------------- Drive ID
|*    || || || ||
|*    || || || ++--------------- always FF
|*    || || ||
|*    || || ++------------------ Object number (HEX) of the icon from the
|*    || ||                      deskicon (or deskcicn.rsc)
|*    || ||
|*    || ++-------------------- y position of the icon on the desktop
|*    ||
|*    ++---------------------- x-position of the icon on the desktop
\*****************************************************************************/

typedef struct
	{
	char 		text[32];				// Text for the drive icon + \0
	char		driveId;				// Cartridge drive letter
	uint8_t		iconId;					// Hex value of the icon in the RSC
	int			x;						// X position on the desktop
	int 		y;						// Y position on the desktop
	} ND_DRIVE;

typedef vec_t(ND_DRIVE*)	vec_drive_t;	// Vector of ND_DRIVE.

/*****************************************************************************\
|*
|* #D_FF_01_000_@_*.*@_@_
|*    || || | |   | |
|*    || || | |   +-+--- always *.*
|*    || || | |
|*    || || +-+--------- always 000
|*    || ||
|*    || ++------------- Object number (HEX) of the icon from the
|*    ||                 deskicon.rsc (or deskcicn.rsc)
|*    ||
|*    ++---------------- always FF
|*
|*
|*
|* Version 2: logged in icon for folders
|*
|* Example:
|*
|* #D_20_20_000_@_FOLDNAME.EXT@_@_
|*    || || | |  |           |
|*    || || | |  +-----------+--- Mask for folder name
|*    || || | |
|*    || || +-+------------------ always 000
|*    || ||
|*    || ++---------------------- Object number (HEX) of the icon from the
|*    ||                          deskicon.rsc (or deskcicn.rsc)
|*    ||
|*    ++------------------------- always same as previous object number
|*
|* Uses iconSpec defined above
\*****************************************************************************/



/*****************************************************************************\
|*
|* #E_18_13_00_16_01_31_00_00_00_00
|*    || || || || || || |         |
|*    || || || || || || +---------+--- 	unused, always 00 00 00 00
|*    || || || || || ||
|*    || || || || || ++--------------- 	Bit 0-2: Number of colors
|*    || || || || || 							000: 2 colors
|*    || || || || || 							001: 4 colors
|*    || || || || || 							010: 16 colors
|*    || || || || || 							011: 256 colors
|*    || || || || || 							100: more than 256 colors
|*    || || || || || 					Bit 3: Column number
|*    || || || || || 							0: 40 columns
|*    || || || || || 							1: 80 columns
|*    || || || || || 					Bit 4-5: unused, always set???
|*    || || || || || 					Bit 6: unused, always unset???
|*    || || || || || 					Bit 7:	0: TT resolution
|*    || || || || || 							1: ST resolution
|*    || || || || ||
|*    || || || || ++------------------ 	00: Line doubling off
|*    || || || || 						01: Line doubling on
|*    || || || ||
|*    || || || ++-------------------- 	Bit 0: Index/unsorted
|*    || || || 									0: off
|*    || || || 									1: on
|*    || || || 							Bit 1: Extras/Cache (TT only)
|*    || || || 									0: off
|*    || || || 									1: on
|*    || || || 							Bit 2: Index/Fit
|*    || || || 									0: off
|*    || || || 									1: on
|*    || || || 							Bit 3: Extras/Desktop configuration:
|*    || || || 								Default directory
|*    || || || 									0: Current window
|*    || || || 									1: Application
|*    || || || 							Bit 4: Extras/Desktop Configuration:
|*    || || || 								Input parameters
|*    || || || 									0: File name only
|*    || || || 									1: Full path
|*    || || ||
|*    || || ++---------------------- unused (should be 0)
|*    || ||
|*    || |+------------------------- 0: current resolution
|*    || | 									1: ST-Low
|*    || | 									2: ST mean
|*    || | 									3: ST-High
|*    || | 									4: TT-Low
|*    || | 									5: TT means
|*    || | 									6: TT-High
|*    || | 									7..15: reserved for future
|*    || | 									(corresponds to the number of the
|*    || | 									VDI Device driver-1)
|*    || |
|*    || +---------------------------- 	0: Blitter off (ST only)
|*    || 								1: Blitter on
|*    ||
|*    ++------------------------------ Bit 0: Extras/Presets:
|*                                     		Overwrite file
|*                                        		0: yes
|*                                             	1: no
|*                                              Note the reverse order
|*                                              versus "copy" and "delete".
|*                                     Bit 1: Cache
|*                                              0: off
|*                                              1: on
|*                                     Bit 2: Insert files
|*                                              0: no
|*                                              1: yes
|*                                     Bit 3: Extras/Preferences: Copy file
|*                                              0: no
|*                                              1: yes
|*                                     Bit 4: Extras/Preferences: Delete file
|*                                              0: no
|*                                              1: yes
|*                                     Bit 5-6: Index
|*                                              0: by name (unless bit is 0
|*                                                 set in the 4th parameter,
|*                                                 because then is "unsorted")
|*                                              1: by date
|*                                              2: by size
|*                                              3: according to Art
|*                                     Bit 7: Index
|
|* Hints:
|*
|* - Under TOS 4.xx and MultiTOS, this line contains 10 values.
|*
|* - Under TOS 2.0x and TOS 3.0x, this line contains the first 4 values.
|*
|* - Under TOS < 2.0 the DESKTOP.INF(!) contains the first two values.
|*
|*
\*****************************************************************************/

typedef enum
	{
	ND_COLOURS_2		= 0,
	ND_COLOURS_4,
	ND_COLOURS_16,
	ND_COLOURS_256,
	ND_COLOURS_MORE
	} ND_COLOURS;

typedef enum
	{
	ND_LINE_DOUBLE_OFF		= 0,
	ND_LINE_DOUBLE_ON
	} ND_LINE_DOUBLE;

typedef enum
	{
	ND_DPY_UNSORTED		= (1<<0),
	ND_DPY_CACHE		= (1<<1),
	ND_DPY_FIT			= (1<<2),
	ND_DPY_APP_DIR		= (1<<3),
	ND_DPY_FULL_PATH	= (1<<4)
	} ND_DPY_FLAGS;

typedef enum
	{
	NS_RES_CURRENT		= 0,
	ND_RES_ST_LOW,
	ND_RES_ST_MED,
	ND_RES_ST_HIGH,
	ND_RES_TT_LOW,
	ND_RES_TT_MED,
	ND_RES_TT_HIGH,
	ND_RES_EXT_CARD
	} ND_RESOLUTION;

typedef enum
	{
	ND_BLITTER_OFF		= 0,
	ND_BLITTER_ON
	} ND_BLITTER;

typedef enum
	{
	ND_PRESET_ASK_OVERWRITE		= (1<<0),
	ND_PRESET_CACHE				= (1<<1),
	ND_PRESET_CREATE_FILES		= (1<<2),
	ND_PRESET_COPY_FILES		= (1<<3),
	ND_PRESET_DELETE_FILES		= (1<<4),
	ND_PRESET_INDEX_DATE		= (1<<5),
	ND_PRESET_INDEX_SIZE		= (1<<6),
	ND_PRESET_INDEX_MASK		= (3<<5)
	} ND_PRESET;
	
typedef struct
	{
	ND_COLOURS		colours;			// How many colours are available
	ND_LINE_DOUBLE	lineDouble;			// Whether we're line-doubling
	ND_DPY_FLAGS	flags;				// How to display/call things
	ND_RESOLUTION	resolution;			// Current display res
	ND_BLITTER		blitter;			// Whether blitter is enabled (ST only)
	ND_PRESET		presets;			// Presets for user preferences
	} ND_DISPLAY_INFO;

/*****************************************************************************\
|* #F_03_04_000_*.TOS@_@_@_
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | +---+--- Standard ending of a TOS program
|*    || || | |
|*    || || +-+--------- always 000
|*    || ||
|*    || ++------------- always 04
|*    ||
|*    |+---------------- Object number (HEX) of the icon from the
|*                       the deskicon.rsc (or deskcicn.rsc)
|*
|* Uses iconSpec defined above
\*****************************************************************************/


/*****************************************************************************\
|* #G_03_FF_000_*.APP@_@_@_
|* #G_03_FF_000_*.PRG@_@_@_
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | |   |
|*    || || | | +---+--- Standard ending of a APP/PRG program
|*    || || | |
|*    || || +-+--------- always 000
|*    || ||
|*    || ++------------- always FF
|*    ||
|*    |+---------------- Object number (HEX) of the icon from the
|*                       the deskicon.rsc (or deskcicn.rsc)
|*
|*
|* #G_03_04_209_D:\ZIP\STZIP.PRG@_*.ZIP@_-v @_xxxxxxxx
|*    || || ||| |				| |    | | |  |      |
|*    || || ||| |				| |    | | |  +------+--- Storage space in bytes
|*    || || ||| |				| |    | | |			  in hex notation which
|*    || || ||| |				| |    | | | 			  the application max.
|*    || || ||| |				| |    | | | 			  may use. Examples:
|*    || || ||| |				| |    | | | 			  00040000 = 256k
|*    || || ||| |				| |    | | | 			  00100000 = 1024k
|*    || || ||| |				| |    | | | 			  empty = unlimited
|*    || || ||| |				| |    | | |
|*    || || ||| |				| |    | +-+------------- Parameters
|*    || || ||| |				| |    |
|*    || || ||| |				| +----+----------------- File type
|*    || || ||| |				|
|*    || || ||| +---------------+------------------------ Application path
|*    || || |||
|*    || || |++------------------------------------------ Function key (0: none)
|*    || || |
|*    || || +-------------------------------------------- Bit 0: Default dir
|*    || || 												0: Current window
|*    || || 												1: Application
|*    || || 											  Bit 1: Start params
|*    || || 												0: file name
|*    || || 												1: Full pathname
|*    || ||
|*    || ++------------------------------------------------ always 04 ?
|*    ||
|*    ++------------------------------------------------ - Object number (HEX)
|*														   of the icon from the
|*                                                         desk[c]icon.rsc
\*****************************************************************************/

typedef enum
	{
	ND_GEM_APP_DIR		= (1<<0),		// set dir to 0:window, 1:app dir
	ND_GEM_FULL_PATH	= (1<<1)		// call with: 0:filename 1: full path
	} ND_GEM_FLAGS;

typedef struct
	{
	int 			maxMem;				// Memory allowed to use, 0=unlimited
	char 			params[PATH_MAX];	// Parameters passed to the command
	char 			spec[32];			// spec for matching to type + \0
	char 			path[PATH_MAX];		// Path to application
	int				fnKey;				// Function key to hot-map
	ND_GEM_FLAGS	flags;				// Flags for launch
	uint8_t			iconId;				// Hex value of the icon in the RSC
	} ND_GEM_ICONSPEC;

typedef vec_t(ND_GEM_ICONSPEC*)	vec_gicon_t;	// Vector of ND_GEM_ICONSPEC

/*****************************************************************************\
|* #Q_41_00_43_40
|*    || || || ||
|*    || || || ++--------- Window colour
|*    || || ++------------ Window pattern
|*    || ++--------------- Desktop color
|*    ++------------------ Desktop pattern
|*
|* Note this isn't quite the same as the "standard" one, because we only have
|* the one resolution, and it's very high colour, compared to the originals
\*****************************************************************************/

typedef struct
	{
	uint8_t 		winColour;			// index into the colour palette
	uint8_t			desktopColour;		// index into the colour palette
	uint8_t			windowPattern;		// 0x00 -> 0x07
	uint8_t			desktopPattern;		// 0x00 -> 0x07
	} ND_WINSTYLE;
	
/*****************************************************************************\
|* #S_00_0A
|*    || ||
|*    || ++--- Height of the character set in points
|*    ||
|*    ++------ ID of the character set (font index, must be monospaced)
\*****************************************************************************/
typedef struct
	{
	int points;							// Height of the character set in pts
	int font;							// Font id to use for system font
	} ND_CHARSET;

/*****************************************************************************\
|* #V_07_05_01_FF___E:\BIN\*.*@_E:\BIN@_
|*    || || || || |          | |       |
|*    || || || || |          | +-------+- Folder icon text
|*    || || || || |          |
|*    || || || || +----------+----------- Access path and file mask
|*    || || || ||
|*    || || || ++------------------------ always FF
|*    || || ||
|*    || || ++--------------------------- Object number (HEX) of the icon
|*    || ||                               the deskicon.rsc (or deskcicn.rsc)
|*    || ||
|*    || ++------------------------------ y position of the icon
|*    ||
|*    ++--------------------------------- x-position of the icon
\*****************************************************************************/

typedef struct
	{
	char 		iconText[32];			// Folder icon text
	char 		pathSpec[PATH_MAX];		// Path to location + wildcards
	uint8_t		iconId;					// Hex value of the icon in the RSC
	int			x;						// X position on the desktop
	int 		y;						// Y position on the desktop
	} ND_LINK;

typedef vec_t(ND_LINK*)	vec_link_t;		// Vector of ND_LINK

/*****************************************************************************\
|* #W_00_00_0A_01_1F_17_00_C:\AUTO\*.PRG@_
|*    || || || || || || || |           |||
|*    || || || || || || || |           |++--- Line end identifier
|*    || || || || || || || |           |
|*    || || || || || || || +-----------+----- Path and file mask of one auto-
|*    || || || || || || ||                    opened directory window or empty
|*    || || || || || || ||
|*    || || || || || || ++------------------- Window status ?
|*    || || || || || ||
|*    || || || || || ||
|*    || || || || || ++---------------------- Window height
|*    || || || || ||
|*    || || || || ++------------------------- Window width
|*    || || || ||
|*    || || || ++---------------------------- y position of the window
|*    || || ||
|*    || || ++------------------------------- x position of the window
|*    || ||
|*    || ++---------------------------------- Position of the vertical slider
|*    ||
|*    ++------------------------------------- Position of the horizontal slider
\*****************************************************************************/

typedef struct
	{
	char 		pathSpec[PATH_MAX];		// Path to location + wildcards
	int			status;					// I have no idea what this does
	int			h;						// window height in 16-pix units
	int 		w;						// window width in 8-pix units
	int			y;						// window top left in 16-pix units
	int 		x;						// window top left in 8-pix units
	int			vs;						// line number of first row
	int			hs;						// line number of first col
	} ND_WINDOW;

typedef vec_t(ND_WINDOW*)	vec_win_t;	// Vector of ND_WINDOW

/*****************************************************************************\
|* Structure to hold all the above information
\*****************************************************************************/
typedef struct
	{
	ND_RS232			serial;			//  #a: All the serial i/o port config
	ND_PRINTER			printcfg;		//  #b: All the printer config
	ND_UI_MISC			misc;			//  #c: Colours, key repeat, mouse clicks
	vec_icon_t			accs;			//  #A: List of accessory configs
	vec_drive_t			carts;			// #C: List of cartridge configs
	vec_icon_t			folders;		//  #D: Window icons in window
	ND_DISPLAY_INFO		dpyInfo;		//  #E: Display configuration
	vec_icon_t			cmds;			//  #F: Window icons for TOS apps
	vec_gicon_t			apps;			//  #G: Window icons for PRG apps
	vec_icon_t			files;			//  #I: Window icons for specific files
	vec_data_t			shortcuts;		//  #K: shortcuts, 1 per menu-entry
	vec_drive_t			drives;			// #M: Desktop icons, storage devices
	ND_ICONSPEC			catchAll;		//  #N: Window icon for specific non-exec
	ND_DRIVE			printer;		// #O: Desktop icon for printer
	vec_gicon_t			cmdParams;		//  #P: Window icons for TTP apps
	ND_WINSTYLE			winStyle;		//  #Q: desk/win patterns and colours
	ND_CHARSET			charset;		// #S: Font-id and size in points
	ND_DRIVE			trash;			// #T: Desktop icon for trash
	vec_link_t			fldrLinks;		// #V: Folder-links on the desktop
	vec_win_t			windows;		//  #W: List of windows on screen
	vec_link_t			fileLinks;		// #X: File-links on the desktop
	vec_gicon_t			appParams;		//  #Y: Window icons for GTP apps
	} ND_INFO;



/*****************************************************************************\
|* Zero out an info structure
\*****************************************************************************/
void _gemInfZero(ND_INFO *info);

/*****************************************************************************\
|* Initialise an info structure so it's ready to use
\*****************************************************************************/
void _gemInfInit(ND_INFO *info);

/*****************************************************************************\
|* Destroy an Inf structure once we've used it up
\*****************************************************************************/
#define _gemInfDeInit _gemInfInit

/*****************************************************************************\
|* Parse a text representation into a structure representation
\*****************************************************************************/
int _gemInfReadData(const char *inf, ND_INFO* info);

/*****************************************************************************\
|* Parse a structure representation into a text representation. Returns an
|* allocated string that must be free'd
\*****************************************************************************/
int _gemInfWriteData(ND_INFO* info, char **inf);

#endif /* shellCfg_h */


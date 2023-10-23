#ifndef __gem_types_header__
#define __gem_types_header__

#include <stdint.h>

/*****************************************************************************\
|* Define a constant so we know it's our version of GEM 
\*****************************************************************************/
#ifndef QGEM
#  define QGEM 1
#endif

/*****************************************************************************\
|* Raster operations 
\*****************************************************************************/
typedef struct
	{
   	void *fd_addr;					/* Pointer to the start of the
                                    	memory block, e.g. the
                                    	screen memory base address  	*/
   	int16_t  fd_w;					/* Width in pixels             		*/
   	int16_t  fd_h;					/* Height in pixels            		*/
   	int16_t  fd_wdwidth;			/* Width of a line in words   		*/
   	int16_t  fd_stand;				/* 0 = Device-specific format  		*/
                                 	/* 1 = Standard format         		*/
   	int16_t  fd_nplanes;			/* Number of planes            		*/
   	int16_t  fd_r1, fd_r2, fd_r3;	/* Reserved, must be 0         		*/
	} MFDB;


/*****************************************************************************\
|* Object types
\*****************************************************************************/
enum
	{
	G_BOX		= 20,
	G_TEXT,
	G_BOXTEXT,
	G_IMAGE,
	G_PROGDEF,
	G_IBOX,
	G_BUTTON,
	G_BOXCHAR,
	G_STRING,
	G_FTEXT,
	G_FBOXTEXT,
	G_ICON,
	G_TITLE,
	G_CICON,
	G_SWBUTTON,
	G_POPUP
	};

typedef struct Object_t
	{
	int16_t    ob_next;   			/* The next object               		*/
	int16_t    ob_head;   			/* First child                   		*/
	int16_t    ob_tail;   			/* Last child                    		*/
	uint16_t   ob_type;   			/* Object type                   		*/
	uint16_t   ob_flags;  			/* Manipulation flags            		*/
	uint16_t   ob_state;  			/* Object status                 		*/
	void       *ob_spec;  			/* More under object type        		*/
	int16_t    ob_x;      			/* X-coordinate of the object    		*/
	int16_t    ob_y;      			/* Y-coordinate of the object    		*/
	int16_t    ob_width;  			/* Width of the object           		*/
	int16_t    ob_height; 			/* Height of the object          		*/
	} OBJECT;


typedef struct
	{
	uint16_t  *ib_pmask;    		/* Pointer to the icon mask           	*/
	uint16_t  *ib_pdata;    		/* Pointer to the icon image          	*/
	int8_t    *ib_ptext;    		/* Pointer to the icon text           	*/
	uint16_t  ib_char;      		/* Character that is to appear in the
									   icon, as well as the foreground
									   and background colour of the Icon  	*/
	uint16_t  ib_xchar;     		/* X-coordinate of the character      	*/
	uint16_t  ib_ychar;     		/* Y-coordinate of the character      	*/
	uint16_t  ib_xicon;     		/* X-coordinate of the icon           	*/
	uint16_t  ib_yicon;     		/* Y-coordinate of the icon           	*/
	uint16_t  ib_wicon;     		/* Width of the icon                  	*/
	uint16_t  ib_hicon;     		/* Height of the icon                 	*/
	int16_t   ib_xtext;     		/* X-coordinate of the text           	*/
	int16_t   ib_ytext;     		/* Y-coordinate of the text           	*/
	uint16_t  ib_wtext;     		/* Width of the text                  	*/
	uint16_t  ib_htext;     		/* Height of the text                 	*/
	uint16_t  ib_resvd;     		/* Reserved                           	*/
	} ICONBLK;


/*****************************************************************************\
|* Define the representation of text-info in the RSC file, used for editable
|* text items
\*****************************************************************************/
typedef struct
	{
    int8_t    *te_ptext;          	/* Pointer to a string              	*/
    int8_t    *te_ptmplt;         	/* Pointer to the string template   	*/
    int8_t    *te_pvalid;         	/* Pointer to the validation string 	*/
    int16_t    te_font;           	/* Font type                        	*/
    int16_t    te_fontid;         	/* GDOS Font ID                     	*/
    int16_t    te_just;           	/* Text alignment:	
                                  	    0 = Ranged left	
                                  	    1 = Ranged right	
                                  	    2 = Centred                     	*/
    int16_t    te_color;          	/* Colour                           	*/
    int16_t    te_fontsize;       	/* GDOS font size in points         	*/
    int16_t    te_thickness;      	/* Border width                     	*/
    int16_t    te_txtlen;         	/* Maximum length of the text       	*/
    int16_t    te_tmplen;         	/* Length of the string template    	*/
	} TEDINFO;	

/*****************************************************************************\
|* Basically an icon without a mask
\*****************************************************************************/
typedef struct
	{
	uint16_t  *bi_pdata;     		/* Pointer to the graphics data  		*/
	int16_t  bi_wb;         		/* Width of the image in bytes   		*/
	int16_t  bi_hl;         		/* Height in lines               		*/
	int16_t  bi_x;          		/* X-position, and               		*/
	int16_t  bi_y;          		/* Y-position of top left corner 		*/
	int16_t  bi_color;      		/* Colour                        		*/
	} BITBLK;				

/*****************************************************************************\
|* Define the representation of a coloured icon
|*
|* 'n' below  is defined as numPlanes * [words in mono icon]
\*****************************************************************************/
typedef struct CICON_t
	{
	uint16_t		 numPlanes;		/* Number of planes this icon is for	*/
	int16_t *		 colData;		/* empty on disk, calculated on load	*/
	int16_t *		 colMask;		/* empty on disk, calculated on load	*/
	int16_t *		 selData;		/* must be !0 if selected data exists	*/
	int16_t * 		 selMask;		/* calculated at load					*/
	struct CICON_t * next;			/* pointer to next in line, or NULL		*/
	} CICON;
	
/*****************************************************************************\
|* Define the representation the coloured icons
\*****************************************************************************/
typedef struct
	{
	ICONBLK 	monoIcon;			/* The ICONBLk for the mono icon 		*/
	CICON *		icons;				/* Structure per resolution				*/
	} CICONBLK;

/*****************************************************************************\
|* Switch info
\*****************************************************************************/
typedef struct
	{
    int8_t  *string;     			/* Perhaps 'TOS|KAOS|MAG!X'            */
    int16_t  num;        			/* Index of current character string   */
    int16_t  maxnum;     			/* Maximum permitted num               */
	} SWINFO;

/*****************************************************************************\
|* Define the internal representation of an RSC file
\*****************************************************************************/

typedef struct
	{
	uint16_t	version;			/* Version, old = 0 or 1, new = 4		*/
	uint32_t	size;				/* File size							*/
	int 		nObjects;			/* Number of OBJECT structures 			*/
	OBJECT *	objects;			/* Pointer to each OBJECT structure 	*/
	int			nCicons;			/* Number of CICON structures			*/
	CICONBLK *	cIcons;				/* The actual CICON data				*/
	int			nTedInfo;			/* Number of TEDINFO structures			*/
	TEDINFO *	tedInfo;			/* The actual TEDINFO data				*/
	int			nIconBlks;			/* Number of ICONBLK structures			*/
	ICONBLK *	iconBlks;			/* The actual ICONBLK data				*/
	int			nBitBlks;			/* Number of BITBLK structures			*/
	BITBLK *	bitBlks;			/* The actual BITBLK data				*/
	int			nStrings;			/* Number of free strings 				*/
	char **		strings;			/* The actual string data				*/
	int			nImages;			/* Number of free images				*/
	BITBLK *	images;				/* The actual image data				*/
	int			nTrees;				/* Number of object trees				*/
	OBJECT **	trees;				/* The actual tree data					*/
	} RscFile;


/*****************************************************************************\
|* Typedefs
\*****************************************************************************/
#ifdef __cplusplus
	#include <vector>
	#include <QList>
	#include <QColor>		// For QRgb
	#include <QString>
	
	typedef std::vector<int16_t>				Payload;
	typedef std::vector<uint8_t>				ByteStream;
	typedef QList<qreal> 						LineStyle;
	typedef QList<qintptr>						HandleList;
	typedef QList<QRgb>							Palette;
	typedef QMap<QString, QString>				StringMap;
#endif


/*****************************************************************************\
|* Line types for vsl_type 
\*****************************************************************************/

#define SOLID			1	/**< solid, see vsl_type() */
#define LDASHED			2	/**< long dashed, see vsl_type() */
#define DOTTED			3	/**< dot, see vsl_type() */
#define DASHDOT			4	/**< dash, dot, see vsl_type() */
#define DASH			5	/**< dash, see vsl_type() */
#define DASHDOTDOT		6	/**< dash, dot, dot, see vsl_type() */
#define USERLINE		7	/**< user-defined with vsl_udsty(), see vsl_type() */

/*****************************************************************************\
|* First sixteen palette colours  
\*****************************************************************************/
#define G_WHITE			0	/**< white */
#define G_BLACK			1	/**< black */
#define G_RED			2	/**< red */
#define G_GREEN			3	/**< green */
#define G_BLUE			4	/**< blue */
#define G_CYAN			5	/**< cyan */
#define G_YELLOW		6	/**< yellow */
#define G_MAGENTA		7	/**< magenta */
#define G_LWHITE		8	/**< light gray */
#define G_LBLACK		9	/**< gray */
#define G_LRED			10	/**< dark red */
#define G_LGREEN		11	/**< dark green */
#define G_LBLUE			12	/**< dark blue */
#define G_LCYAN			13	/**< dark cyan */
#define G_LYELLOW		14	/**< dark yellow */
#define G_LMAGENTA		15	/**< dark magenta */

/*****************************************************************************\
|* Marker types for vsm_type  
\*****************************************************************************/
#define MRKR_DOT		1	/**< dot marker type, see vsm_type() */
#define MRKR_PLUS 		2	/**< plus marker type, see vsm_type() */
#define MRKR_ASTERISK	3	/**< asterisk marker type, see vsm_type() */
#define MRKR_BOX		4	/**< square marker type, see vsm_type() */
#define MRKR_CROSS		5	/**< diagonal cross marker type, see vsm_type() */
#define MRKR_DIAMOND	6	/**< diamond marker type, see vsm_type() */
#define MRKR_CIRCLE		7	/**< circle marker type, see vsm_type() */



/*****************************************************************************\
|* Style of fill to use  
\*****************************************************************************/
#define FIS_HOLLOW		0	/**< hollow interior, see vsf_interior() */
#define FIS_SOLID		1	/**< solid interior, see vsf_interior() */
#define FIS_PATTERN		2	/**< patterned fill, see vsf_interior() */
#define FIS_HATCH		3	/**< hatched fill, see vsf_interior() */
#define FIS_USER		4	/**< user-defined fill, see vsf_interior() */
#define FIS_ERROR		5

/*****************************************************************************\
|* Patern indexes : These are valid for fill-type == pattern 
\*****************************************************************************/
#define PT_DOTS1		1	/**< diagonally offset dots */
#define PT_DOTS2		2	/**< black grid of dots on white */
#define PT_LINES1		3	/**< criss-crossing diagonal lines */
#define PT_DOTS_DARK	4	/*** white dots on black */
#define PT_DOTS_VDARK	5	/*** white dots on black */
#define PT_LINES1_DARK	6	/**< criss-crossing diagonal lines */
#define PT_LINES1_VDARK	7	/**< criss-crossing diagonal lines */
#define PT_SOLID		8	/**< solid colour */
#define PT_BRICKS		9	/**< horizontal brick pattern */
#define PT_BRICKS_DIAG	10	/**< diagonal brick pattern */
#define PT_CIRCUMFLEX	11	/**< up-arrow types without line */
#define PT_SHELLS		12	/**< shell patterns */
#define PT_DIAG_DASH	13	/**< Diagonal separated dashed lines */
#define PT_UGLY			14	/**< I mean, no idea... */
#define PT_DOTS_SPARSE	15	/**< few dots */
#define PT_WEAVE		16	/**< Kind of basket-weave */
#define PT_LINE_DOT		17	/**< diagonal up/down and dot pattern */
#define PT_SQUARE_DOT	18	/**< squares with dots in */
#define PT_BALLS		19	/**< shaded balls */
#define PT_ROOF1		20	/**< roof-tiles */
#define PT_ROOF2		21	/**< angled roof tiles */
#define PT_CHECKER		22	/**< chess-board type pattern */
#define PT_DIAMONDS		23	/**< 45-degree chess-board pattern */
#define PT_CHEVRON		24	/**< horizontal <<< pattern */

/*****************************************************************************\
|* Patern indexes : These are valid for fill-type == hatch 
\*****************************************************************************/
#define PT_DIAG1		1	/**< diagonal lines */
#define PT_DIAG2		2	/**< diagonal lines, closer together */
#define PT_CROSS_HATCH	3	/**< diagonal lines in both directions */
#define PT_VERTICAL		4	/**< vertical lines */
#define PT_HORIZONTAL	5	/**< horizontal lines */
#define PT_SQUARE		6	/**< horiz and vertical lines */
#define PT_DIAG3		7	/**< spread-out diagonal lines */
#define PT_DIAG3_BOLD	8	/**< spread-out bold diagonal lines */
#define PT_CROSS_HATCH2	9	/**< diagonal lines in both directions */
#define PT_VERTICAL2	10	/**< vertical lines, spread out */
#define PT_HORIZONTAL2	11	/**< horizontal lines, spread out */
#define PT_SQUARE2		12	/**< horiz and vertical lines */


/*****************************************************************************\
|* Co0ordinate types 
\*****************************************************************************/
#define COORD_NDC		0	/**< Use NDC co-ords, only in metafiles */
#define COORD_RASTER	2	/**< Use raster co-ords, only in metafiles */

/*****************************************************************************\
|* Page-format types 
\*****************************************************************************/
#define PG_DEFAULT		0	/**< Use the default page-format */
#define PG_DIN_A3		1	/**< Use A3 page-format */
#define PG_DIN_A4		2	/**< Use A4 page-format */
#define PG_DIN_A5		3	/**< Use A5 page-format */
#define PG_DIN_B5		4	/**< Use B5 page-format */
#define PG_LETTER		16	/**< Use US Letter page-format */
#define PG_HALF			17	/**< Use US half-letter page-format */
#define PG_LEGAL		18	/**< Use US legal page-format */
#define PG_DOUBLE		19	/**< Use 2xUS-letter page-format */
#define PG_BROADSHEET	20	/**< Use US broadssheet page-format */

/*****************************************************************************\
|* GDP types 
\*****************************************************************************/
#define GDP_LINE 		0	/**< Draw a line */
#define GDP_MARKER		1	/**< Draw a marker */
#define GDP_TEXT		2	/**< Draw text */
#define GDP_FILL		3	/**< Fill */
#define GDP_NONE		4	/**< Draw nothing */

/*****************************************************************************\
|* GDPs 
\*****************************************************************************/
#define GDP_BAR 			1
#define GDP_ARC				2
#define GDP_PIESLICE		3
#define GDP_FILLED_CIRCLE	4
#define GDP_FILLED_ELLIPSE	5
#define GDP_ELLIPTICAL_ARC	6
#define GDP_ELLIPTICAL_PIE	7
#define GDP_ROUNDED_RECT	8
#define GDP_FILLED_RRECT	9
#define GDP_JUSTIFIED_TEXT	10
#define GDP_LIST_COMPLETE	-1

/*****************************************************************************\
|* Writing modes 
\*****************************************************************************/
#define WR_REPLACE		1	/**< Default white bg, then draw */
#define WR_TRANSPARENT	2	/**< Use alpha  */
#define WR_XOR			3	/**< XOR mode */
#define WR_REV_TRANS	4	/**< Reverse-tansparent */

/*****************************************************************************\
|* Line-end styles 
\*****************************************************************************/
#define CAP_SQUARE		0
#define CAP_ARROW		1
#define CAP_ROUND		2

/*****************************************************************************\
|* Text effects 
\*****************************************************************************/
#define TXT_NORMAL		0
#define TXT_BOLD		(1<<0)
#define TXT_LIGHT		(1<<1)
#define TXT_ITALIC		(1<<2)
#define TXT_UNDERLINE	(1<<3)
#define TXT_OUTLINE		(1<<4)
#define TXT_MASK		(TXT_BOLD|TXT_LIGHT|TXT_ITALIC|TXT_UNDERLINE|TXT_OUTLINE)

/*****************************************************************************\
|* Text alignment, horizontal (TA_FULL is an extension) then vertical
\*****************************************************************************/
enum
	{
	TA_LEFT				= 0,
	TA_CENTER, 
	TA_RIGHT,
	TA_FULL
	};

enum 
	{  
	TA_BASE				= 0,
	TA_HALF,   
	TA_ASCENT, 
	TA_BOTTOM, 
	TA_DESCENT,
	TA_TOP    
	};
	


/*****************************************************************************\
|* Gem event types 
\*****************************************************************************/
enum
	{
	ETYP_MOUSE_BTN			= (1<<0),
	ETYP_MOUSE_ENTER		= (1<<1),
	ETYP_MOUSE_EXIT			= (1<<2),
	ETYP_MOUSE_MOVE			= (1<<3),
	ETYP_MOUSE_WHEEL		= (1<<4),
	ETYP_KEYBOARD			= (1<<5),
	};
	
/*****************************************************************************\
|* Input devices 
\*****************************************************************************/
enum 
	{
	INP_LOCATOR			= 1,
	INP_VALUATOR,
	INP_CHOICE,
	INP_STRING,
	INP_MAX
	};

#define INPUT_REQUEST	1
#define INPUT_SAMPLE	2

/*****************************************************************************\
|* Key modifiers 
\*****************************************************************************/
#define MDFY_SHIFT		3
#define MDFY_CTRL		4
#define MDFY_ALT		8
#define MDFY_META		16
#define MDFY_KEYPAD		32

/*****************************************************************************\
|* Keys. Basically ASCII + extra bits
|* high byte is:
|*  0x01 : 	"special" key
|*  0x02 : 	"shift key pressed"
|*  0x04 : 	"ctrl  key pressed"
|*  0x08 : 	"meta  key pressed"
|*  0x10 : 	"alt   key pressed"
|*
\*****************************************************************************/
enum 
	{
	KY_ESCAPE 		= 27,
	KY_TAB	  		= 10,
	KY_BACKTAB		= 0x100 + 10,
	KY_BACKSPACE	= 8,
	KY_RETURN		= 13,
	KY_DELETE		= 127,
	KY_PAUSE		= 0x100 + 128,
	KY_PRINT		= 0x100 + 129,
	KY_SYSREQ		= 0x100 + 130,
	KY_CLEAR		= 0x100 + 131,
	KY_HOME		 	= 0x100 + 132,
	KY_INSERT		= 0x100 + 133,
	KY_END			= 0x100 + 134,
	KY_LEFT			= 0x100 + 135,
	KY_RIGHT		= 0x100 + 136,
	KY_UP			= 0x100 + 137,
	KY_DOWN			= 0x100 + 138,
	KY_PAGEUP		= 0x100 + 139,
	KY_PAGEDN		= 0x100 + 140,
	KY_CAPSLOCK		= 0x100 + 141,
	KY_NUMLOCK		= 0x100 + 142,
	KY_SCROLLLOCK	= 0x100 + 143,
	KY_F1			= 0x100 + 144,		// + the next 11 for F1..12
	KY_HELP			= 0x100 + 156,
	
	};



/*****************************************************************************\
|* Raster operations 
\*****************************************************************************/
enum
	{
	ALL_WHITE		= 0,
	S_AND_D,
	S_AND_NOTD,
	S_ONLY,
	NOTS_AND_D,
	D_ONLY,
	S_XOR_D,
	S_OR_D,
	NOT_SORD,
	NOT_SXORD,
	D_INVERT,
	NOT_D,
	S_OR_NOTD,
	NOTS_OR_D,
	NOT_SANDD,
	ALL_BLACK	
	};

enum 
	{
	MFDB_DEVICE	= 0,
	MFDB_STANDARD
	};

/*****************************************************************************\
|* events  
\*****************************************************************************/
typedef void (*GemTimerFunction)(void);
typedef void (*GemMouseButtonFunction)(int btnState);
typedef void (*GemMouseMoveFunction)(int x, int y);


/*****************************************************************************\
|* Resource types  
\*****************************************************************************/
enum
	{
	R_TREE				= 0,
	R_OBJECT,
	R_TEDINFO,
	R_ICONBLK,
	R_BITBLK,
	R_STRING,
	R_IMAGEDATA,
	R_OBSPEC,
	R_TEPTEXT,
	R_TEPTMPLT,
	R_TEPVALID,
	R_IBPMASK,
	R_IBPDATA,
	R_IBPTEXT,
	R_BIPDATA,
	R_FRSTR,
	R_FRIMG,
	R_MAX
	};

#endif /* ! __gem_types_header__ */

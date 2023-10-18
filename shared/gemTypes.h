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
	
	typedef std::vector<int16_t>	Payload;
	typedef std::vector<uint8_t>	ByteStream;
	typedef QList<qreal> 			LineStyle;
	typedef QList<qintptr>			HandleList;
	typedef QList<QRgb>				Palette;
#endif

#endif /* ! __gem_types_header__ */

//
//  gemobject.h
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#ifndef gemobject_h
#define gemobject_h

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

#endif /* gemobject_h */

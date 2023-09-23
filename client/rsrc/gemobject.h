//
//  gemobject.h
//  rsrc
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#ifndef gemobject_h
#define gemobject_h

typedef struct
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

#endif /* gemobject_h */

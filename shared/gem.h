#ifndef __gem_defines_header__
#define __gem_defines_header__

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
#define TXT_LIGHT		(1<<2)
#define TXT_ITALIC		(1<<3)
#define TXT_UNDERLINE	(1<<4)
#define TXT_OUTLINE		(1<<5)
#define TXT_MASK		(TXT_BOLD|TXT_LIGHT|TXT_ITALIC|TXT_UNDERLINE|TXT_OUTLINE)

/*****************************************************************************\
|* Text alignment 
\*****************************************************************************/
#define ALGN_LEFT		0
#define ALGN_CENTER		1
#define ALGN_RIGHT		2

#define ALGN_BASELINE	0
#define ALGN_HALF		1
#define ALGN_ASCENT		2
#define ALGN_BOTTOM		3
#define ALGN_DESCENT	4
#define ALGN_TOP		5


/*****************************************************************************\
|* Key modifiers 
\*****************************************************************************/
#define MDFY_SHIFT		1
#define MDFY_CTRL		2
#define MDFY_ALT		4
#define MDFY_META		8
#define MDFY_KEYPAD		16


#endif /* !__gem_defines_header__ */
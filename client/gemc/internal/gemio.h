//
//  gemio.h
//  gemc
//
//  Created by ThrudTheBarbarian on 9/7/23.
//

#ifndef gemio_h
#define gemio_h

#include <stdio.h>
#include "gemmsg.h"

#define MSG_REPLY(x) ((x) + MSG_REPLY_OFFSET)

/*****************************************************************************\
|* Enumerate the message-types for going back and forth to the server
\*****************************************************************************/
enum
	{
	MSG_V_CLRWK			= 3,
	MSG_VQ_CHCELLS		= 5001,	/* Really 5.1 etc. */
	MSG_VQ_EXIT_CUR		= 5002,
	MSG_V_ENTER_CUR		= 5003,
	MSG_V_CURUP			= 5004,
	MSG_V_CURDOWN		= 5005,
	MSG_V_CURRIGHT		= 5006,
	MSG_V_CURLEFT		= 5007,
	MSG_V_CURHOME		= 5008,
	MSG_V_EEOS			= 5009,
	MSG_V_EEOL			= 5010,
	MSG_VS_CURADDRESS	= 5011,
	MSG_V_CURTEXT		= 5012,
	MSG_V_RVON			= 5013,
	MSG_V_RVOFF			= 5014,
	MSG_VQ_CURADDRESS	= 5015,
	MSG_VQ_TABSTATUS	= 5016,	/* not implemented */
	MSG_V_HARDCOPY		= 5017, /* not implemented */
	MSG_V_DSPCUR		= 5018,
	MSG_V_RMCUR			= 5019,
	MSG_V_PLINE			= 6,
	MSG_V_PMARKER		= 7,
	MSG_V_GTEXT			= 8,
	MSG_V_FILLAREA		= 9,
	MSG_V_BAR			= 11001,
	MSG_V_ARC			= 11002,
	MSG_V_PIE			= 11003,
	MSG_V_CIRCLE		= 11004,
	MSG_V_ELLIPSE		= 11005,
	MSG_V_ELLARC		= 11006,
	MSG_V_ELLPIE		= 11007,
	MSG_V_RBOX			= 11008,
	MSG_V_RFBOX			= 11009,
	MSG_V_JUSTIFIED		= 11010,
	MSG_VST_HEIGHT	 	= 12,
	MSG_VST_ROTATION	= 13,
	MSG_VS_COLOR		= 14,
	MSG_VSL_TYPE		= 15,
	MSG_VSL_WIDTH		= 16,
	MSG_VSL_COLOR		= 17,
	MSG_VSM_TYPE		= 18,
	MSG_VSM_HEIGHT		= 19,
	MSG_VSM_COLOR		= 20,
	MSG_VST_FONT		= 21,
	MSG_VST_COLOR		= 22,
	MSG_VSF_INTERIOR	= 23,
	MSG_VSF_STYLE		= 24,
	MSG_VSF_COLOR		= 25,
	MSG_VQ_COLOR		= 26,
	MSG_V_LOCATOR		= 28,
	MSG_VSWR_MODE		= 32,
	MSG_VSIN_MODE		= 33,
	MSG_VQL_ATTRIBUTES	= 35,
	MSG_VQM_ATTRIBUTES	= 36,
	MSG_VQF_ATTRIBUTES	= 37,
	MSG_VQT_ATTRIBUTES	= 38,
	MSG_VST_ALIGNMENT	= 39,
	MSG_VS_PIXRGB		= 59,
	MSG_VQ_PIXRGB		= 60,
	MSG_V_OPNVWK		= 100,
	MSG_VQ_EXTND		= 102,
	MSG_V_CONTOURFILL	= 103,
	MSG_VSF_PERIMETER	= 104,
	MSG_V_GET_PIXEL		= 105,
	MSG_VST_EFFECTS		= 106,
	MSG_VST_POINT		= 107,
	MSG_VSL_ENDS		= 108,
	MSG_VRO_CPYFM		= 109,
	MSG_VSC_FORM		= 111,
	MSG_VSF_UDPAT		= 112,
	MSG_VSF_UDSTY		= 113,
	MSG_VQIN_MODE		= 115,
	MSG_VQT_EXTENT		= 116,
	MSG_VQT_WIDTH		= 117,
	MSG_VEX_TIMV		= 118,
	MSG_VST_LOAD_FONTS	= 119,
	MSG_VRT_CPYFM		= 121,
	MSG_VS_CLIP			= 129,
	MSG_VQT_NAME		= 130,
	
	MSG_VQ_COLOURS		= 2026,
	
	MSG_REPLY_OFFSET	= 8192,


	EVT_MOUSE_MOVE		= 17000,
	EVT_MOUSE_DOWN		= 17001,
	EVT_MOUSE_UP		= 17002,
	EVT_KEY_PRESS		= 17010,
	EVT_TIMER			= 17020,
	
	EVT_FILTER_SET		= 17100,
	};
	
/*****************************************************************************\
|* Check to see if the connection has been made to the server
\*****************************************************************************/
int _gemIoIsConnected(void);

/*****************************************************************************\
|* Connect to the server
\*****************************************************************************/
int _gemIoConnect(void);

/*****************************************************************************\
|* Write to the socket
\*****************************************************************************/
int _gemIoWrite(GemMsg *msg);

/*****************************************************************************\
|* Read from the socket
\*****************************************************************************/
int _gemIoRead(GemMsg *msg);

/*****************************************************************************\
|* Read from the socket, looking for a specific response
\*****************************************************************************/
int _gemIoWaitForMessageOfType(GemMsg *msg, int16_t type);
	
/*****************************************************************************\
|* Function to request a blocking read of a message, filtering on a list of
|* message types. This always tries to read from the socket
\*****************************************************************************/
int _gemIoWaitForMessages(GemMsg *msg, vec_word_t *types);


/*****************************************************************************\
|* Return the current GEM message event-filter
\*****************************************************************************/
int _gemIoEventFilter(void);

/*****************************************************************************\
|* Set the current GEM message event-filter, and tell the server
\*****************************************************************************/
void _gemIoSetEventFilter(int value);

/*****************************************************************************\
|* Sample the current mouse/modifier state, any of these can be NULL
\*****************************************************************************/
void _gemIoMouseState(int16_t *x, int16_t *y, int16_t *btns, int16_t *mods);

/*****************************************************************************\
|* Disconnect from the server
\*****************************************************************************/
void _gemIoDisconnect(void);

#endif /* gemio_h */

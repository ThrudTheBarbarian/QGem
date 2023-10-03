#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QByteArray>
#include <QIODevice>
#include <QMouseEvent>
#include <QKeyEvent>

#include "properties.h"
#include "macros.h"

#define MSG_REPLY(x) ((x) + ClientMsg::MSG_REPLY_OFFSET)

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ClientMsg
	{
	/*************************************************************************\
	|* Public typedefs, defines, etc.
	\*************************************************************************/
	public:
		enum
		{
		MSG_REPLY_OFFSET	= 8192,
		INVALID				= -1,
		V_CLRWK				= 3,
		VQ_CHCELLS			= 5001,		// Really 5.1
		VQ_EXIT_CUR			= 5002,
		V_ENTER_CUR			= 5003,
		V_CURUP				= 5004,
		V_CURDOWN			= 5005,
		V_CURRIGHT			= 5006,
		V_CURLEFT			= 5007,
		V_CURHOME			= 5008,
		V_EEOS				= 5009,
		V_EEOL				= 5010,
		VS_CURADDRESS		= 5011,
		V_CURTEXT			= 5012,
		V_RVON				= 5013,
		V_RVOFF				= 5014,
		VQ_CURADDRESS		= 5015,
		VQ_TABSTATUS		= 5016,		// Not implemented
		V_HARDCOPY			= 5017,		// Not implemented
		V_DSPCUR			= 5018,
		V_RMCUR				= 5019,
		V_PLINE				= 6,
		V_PMARKER			= 7,
		V_GTEXT				= 8,
		V_FILLAREA			= 9,
		V_BAR				= 11001,	// Really 11.1
		V_ARC				= 11002,
		V_PIE				= 11003,
		V_CIRCLE			= 11004,
		V_ELLIPSE			= 11005,
		V_ELLARC			= 11006,
		V_ELLPIE			= 11007,
		V_RBOX				= 11008,
		V_RFBOX				= 11009,
		V_JUSTIFIED			= 11010,
		VST_HEIGHT			= 12,
		VST_ROTATION		= 13,
		VS_COLOR			= 14,
		VSL_TYPE			= 15,
		VSL_WIDTH			= 16,
		VSL_COLOR			= 17,
		VSM_TYPE			= 18,
		VSM_HEIGHT			= 19,
		VSM_COLOR			= 20,
		VST_FONT			= 21,
		VST_COLOR			= 22,
		VSF_INTERIOR		= 23,
		VSF_STYLE			= 24,
		VSF_COLOR			= 25,
		VQ_COLOR			= 26,
		VSWR_MODE			= 32,
		VSIN_MODE			= 33,
		VQL_ATTRIBUTES		= 35,
		VQM_ATTRIBUTES		= 36,
		VQF_ATTRIBUTES		= 37,
		VQT_ATTRIBUTES		= 38,
		VST_ALIGNMENT		= 39,
		VS_PIXRGB			= 59,
		VQ_PIXRGB			= 60,
		V_OPNVWK			= 100,
		VQ_EXTND			= 102,
		V_CONTOURFILL		= 103,
		VSF_PERIMETER		= 104,
		V_GET_PIXEL			= 105,
		VST_EFFECTS			= 106,
		VST_POINT			= 107,
		VSL_ENDS			= 108,
		VRO_CPYFM			= 109,
		VSC_FORM			= 111,
		VSF_UDPAT			= 112,
		VST_LOAD_FONTS		= 119,
		VS_CLIP				= 129,
		VQT_NAME			= 130,

		VQ_COLOURS			= 2026,

		EVT_MOUSE_MOVE		= 17000,
		EVT_MOUSE_DOWN		= 17001,
		EVT_MOUSE_UP		= 17002,
		EVT_KEY_PRESS		= 17010,

		EVT_FILTER_SET		= 17100,
		};

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSET(Payload, payload, Payload);			// Message payload
	GETSET(int16_t, type, Type);				// Received type

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		ClientMsg();
		ClientMsg(int16_t type);
		ClientMsg(int16_t type, Payload payload);
		~ClientMsg() {}

		/*********************************************************************\
		|* Append to a payload
		\*********************************************************************/
		bool append(int16_t value);
		void append(uint8_t *data, uint32_t num);
		bool append(int16_t *value, int num);
		inline bool append(Payload list)
			{
			return append(list.data(), (int)list.size());
			}

		/*********************************************************************\
		|* Populate the message from a socket
		\*********************************************************************/
		bool read(QIODevice *dev);

		/*********************************************************************\
		|* Populate the message from a mouse event
		\*********************************************************************/
		void fromMouseEvent(QMouseEvent* ev, int16_t type);

		/*********************************************************************\
		|* Populate the message from a key event
		\*********************************************************************/
		void fromKeyEvent(QKeyEvent* ev, int16_t type);

		/*********************************************************************\
		|* Clear the properties so we can re-use the message
		\*********************************************************************/
		void clear(void);

		/*********************************************************************\
		|* Serialise to a byte-stream
		\*********************************************************************/
		QByteArray encode(void);

		/*********************************************************************\
		|* Fetch a byte-array encoded as {length, data} in the word-stream.
		|* returns the number of words consumed.
		\*********************************************************************/
		int fetchData(int idx, QByteArray& ba);

		/*********************************************************************\
		|* De-serialise from a byte-stream
		\*********************************************************************/
		bool decode(int16_t words, QByteArray& inputData);

	};

#endif // CLIENTMSG_H

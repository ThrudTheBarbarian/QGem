#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QByteArray>
#include <QIODevice>

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
		VST_HEIGHT			= 12,
		VST_ROTATION		= 13,
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
		VSWR_MODE			= 32,
		VST_ALIGNMENT		= 39,
		V_OPNVWK			= 100,
		VSF_PERIMETER		= 104,
		VST_EFFECTS			= 106,
		VST_POINT			= 107,
		VSL_ENDS			= 108,
		VST_LOAD_FONTS		= 119,
		VS_CLIP				= 129,
		VQT_NAME			= 130,
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
		void append(uint8_t *data, int num);
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
		|* Clear the properties so we can re-use the message
		\*********************************************************************/
		void clear(void);

		/*********************************************************************\
		|* Serialise to a byte-stream
		\*********************************************************************/
		QByteArray encode(void);

		/*********************************************************************\
		|* Fetch a byte-array encoded as {length, data} in the word-stream
		\*********************************************************************/
		void fetchData(int idx, QByteArray& ba);

		/*********************************************************************\
		|* De-serialise from a byte-stream
		\*********************************************************************/
		bool decode(int16_t words, QByteArray& inputData);

	};

#endif // CLIENTMSG_H

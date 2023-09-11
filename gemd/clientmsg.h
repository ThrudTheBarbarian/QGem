#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QByteArray>
#include <QIODevice>

#include "properties.h"
#include "macros.h"

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
		V_OPNVWK			= 100,
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

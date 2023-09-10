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
		V_OPNVWK			= 100
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
		|* De-serialise from a byte-stream
		\*********************************************************************/
		bool decode(int16_t words, QByteArray& inputData);

	};

#endif // CLIENTMSG_H

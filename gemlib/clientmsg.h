#ifndef CLIENTMSG_H
#define CLIENTMSG_H

#include <QByteArray>
#include <QObject>

#include "properties.h"
#include "macros.h"

/*****************************************************************************\
|* Class declaration
\*****************************************************************************/
class ClientMsg : public QObject
	{
	Q_OBJECT

	/*************************************************************************\
	|* Public typedefs, defines, etc.
	\*************************************************************************/
	public:
		enum
		{
		TYPE_INVALID	= -1,
		TYPE_V_OPNVWK	= 100
		};

	/*************************************************************************\
	|* Private state
	\*************************************************************************/
	private:

	/*************************************************************************\
	|* Properties
	\*************************************************************************/
	GETSET(Payload, payload, Payload);			// Message payload
	GET(int16_t, checksum);						// Message checksum
	GET(int16_t, type);							// Received type

	public:
		/*********************************************************************\
		|* Constructor / Destructor
		\*********************************************************************/
		explicit ClientMsg(QObject *parent = nullptr);
		explicit ClientMsg(int16_t type, QObject *parent = nullptr);
		explicit ClientMsg(int16_t type, Payload payload, QObject *parent = nullptr);

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
		|* Serialise to a byte-stream
		\*********************************************************************/
		QByteArray encode(void);

		/*********************************************************************\
		|* De-serialise from a byte-stream
		\*********************************************************************/
		bool decode(int16_t words, QByteArray& inputData);

	};

#endif // CLIENTMSG_H

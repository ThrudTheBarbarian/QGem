#include <QIODevice>

#include "clientmsg.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ClientMsg::ClientMsg()
	{
	}

ClientMsg::ClientMsg(int16_t type)
	{
	_payload.push_back(type & 0xFF);
	_payload.push_back((type >> 8) & 0xFF);
	}

ClientMsg::ClientMsg(int16_t type, Payload payload)
		  :_payload(payload)
	{
	_payload.push_back(type);
	_payload.insert(_payload.end(), payload.begin(), payload.end());
	}

/*****************************************************************************\
|* Serialise the message to something we can transmit
\*****************************************************************************/
QByteArray ClientMsg::encode(void)
	{
	int len = (int)(_payload.size() + 2);
	int16_t buf[len];

	buf[0] = htons((int16_t)(len-1));
	buf[1] = htons(_type);

	int idx = 2;
	for (int16_t word : _payload)
		buf[idx++] = htons(word);

	return QByteArray((const char *)(&(buf[0])), len*2);
	}

/*****************************************************************************\
|* De-serialise the message to the original form
\*****************************************************************************/
bool ClientMsg::decode(int16_t words, QByteArray& ba)
	{
	_payload.clear();
	bool ok			= false;
	_type			= INVALID;
	int len			= (int)ba.size() / 2 - 1;
	int16_t *ptr	= (int16_t *)(ba.data());

	if (len == words)
		{
		_type = ntohs(*ptr++);
		for (int i=0; i<len; i++)
			_payload.push_back(ntohs(*ptr++));

		ok = true;
		}
	return ok;
	}


/*****************************************************************************\
|* De-serialise the message to the original form
\*****************************************************************************/
void ClientMsg::clear(void)
	{
	_payload.clear();
	_type = 0;
	}

/*****************************************************************************\
|* Read a message from a socket
\*****************************************************************************/
bool ClientMsg::read(QIODevice *dev)
	{
	bool ok = false;

	if (dev->bytesAvailable() > 2)
		{
		/*********************************************************************\
		|* Get the size of the payload, expressed in words
		\*********************************************************************/
		int16_t length;
		dev->read((char *)(&length), 2);
		length = ntohs(length);

		if (dev->bytesAvailable() >= length*2)
			{
			_payload.clear();
			dev->read((char *)(&_type), 2);
			_type = ntohs(_type);
			length --;

			QByteArray msgData	= dev->read(length*2);
			int16_t *data = (int16_t *)(msgData.data());
			for (int i=0; i<length; i++)
				_payload.push_back(ntohs(*data ++));
			ok = 1;
			}
		else
			{
			WARN("Insufficient data for msg type 0x%04X "
				 "(%d required, %d available)",
				 _type, length*2, (int)dev->bytesAvailable());
			}
		fprintf(stderr, "Got message of type: %d\n", _type);
		}

	return ok;
	}

/*****************************************************************************\
|* Append an int to the payload
\*****************************************************************************/
bool ClientMsg::append(int16_t value)
	{
	_payload.push_back(value);
	return true;
	}

bool ClientMsg::append(int16_t *data, int num)
	{
	for (int i=0; i<num; i++)
		_payload.push_back(*data++);
	return true;
	}

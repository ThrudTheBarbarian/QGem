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
	QByteArray ba;
	int16_t checksum	= 0;

	int16_t len			= (int)(_payload.size() +1);
	ba.append(len & 0xFF);
	ba.append((len >> 8) & 0xFF);

	for (int16_t word : _payload)
		{
		ba.append(word & 0xFF);
		ba.append((word >> 8) & 0xFF);
		checksum += word;
		}
	ba.append(checksum & 0xFF);
	ba.append((checksum >> 8) & 0xFF);

	return ba;
	}

/*****************************************************************************\
|* De-serialise the message to the original form
\*****************************************************************************/
bool ClientMsg::decode(int16_t words, QByteArray& ba)
	{
	bool ok = false;
	_payload.clear();
	_type	= TYPE_INVALID;
	int len	= (int)ba.size();

	if ((len >> 1) == words)
		{
		int16_t checksum = 0;
		_type = ba.at(0) + (256 * ba.at(1));
		for (int i=2; i<len-2; i+=2)
			{
			int16_t word = ba.at(i) + (256 * ba.at(i+1));
			_payload.push_back(word);
			checksum += word;
			}

		int16_t streamChecksum = ba.at(len-2) + 256 * (ba.at(len-1));
		if (streamChecksum == checksum)
			ok = true;
		}
	return ok;
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
			}
		else
			WARN("Insufficient data for msg type 0x%04X "
				 "(%d required, %d available)",
				 _type, length*2, (int)dev->bytesAvailable());
		}

	fprintf(stderr, "Got message of type: %d\n", _type);
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

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
		|* Get the size of the payload
		\*********************************************************************/
		uint8_t sizebuf[2];
		dev->read((char *)sizebuf, 2);
		int length = sizebuf[0] + sizebuf[1] * 256;

		if (dev->bytesAvailable() >= length)
			{
			_payload.clear();
			QByteArray msgData	= dev->read(length);
			int16_t checksum	= 0;
			_type				= msgData[0] + 256 * msgData[1];

			for (int i=2; i<length-2; i+=2)
				{
				int16_t word = msgData[i] + 256 * msgData[i+1];
				_payload.push_back(word);
				checksum += word;
				}

			int16_t streamSum = msgData[length-2] + msgData[length-1] * 256;
			ok = (streamSum == checksum);
			if (!ok)
				WARN("Checksum mismatch for msg type 0x%04X", _type);
			}
		else
			WARN("Insufficient data for msg type 0x%04X "
				 "(%d required, %d available)",
				 _type, length, (int)dev->bytesAvailable());
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

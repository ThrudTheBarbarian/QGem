#include "clientmsg.h"

/*****************************************************************************\
|* Class definition
\*****************************************************************************/
ClientMsg::ClientMsg(QObject *parent)
		  :QObject{parent}
	{
	}

ClientMsg::ClientMsg(int16_t type, QObject *parent)
	:QObject{parent}
	{
	_payload.push_back(type & 0xFF);
	_payload.push_back((type >> 8) & 0xFF);
	}

ClientMsg::ClientMsg(int16_t type, Payload payload, QObject *parent)
	:QObject{parent}
	,_payload(payload)
	{
	_payload.push_back(type);
	_payload.insert(_payload.end(), payload.begin(), payload.end());
	}

/*****************************************************************************\
|* Serialise the message to something we can transmit
\*****************************************************************************/
ByteStream ClientMsg::encode(void)
	{
	ByteStream bs;
	int16_t checksum	= 0;

	int16_t len			= (int)(_payload.size() +1);
	bs.push_back(len & 0xFF);
	bs.push_back((len >> 8) & 0xFF);

	for (int16_t word : _payload)
		{
		bs.push_back(word & 0xFF);
		bs.push_back((word >> 8) & 0xFF);
		checksum += word;
		}
	bs.push_back(checksum & 0xFF);
	bs.push_back((checksum >> 8) & 0xFF);

	return bs;
	}

/*****************************************************************************\
|* De-serialise the message to the original form
\*****************************************************************************/
bool ClientMsg::decode(int16_t words, ByteStream& bs)
	{
	bool ok = false;
	_payload.clear();
	_type	= TYPE_INVALID;
	int len	= (int)bs.size();

	if ((len >> 1) == words)
		{
		int16_t checksum = 0;
		_type = bs.at(0) + (256 * bs.at(1));
		for (int i=2; i<len-2; i+=2)
			{
			int16_t word = bs.at(i) + (256 * bs.at(i+1));
			_payload.push_back(word);
			checksum += word;
			}

		int16_t streamChecksum = bs.at(len-2) + 256 * (bs.at(len-1));
		if (streamChecksum == checksum)
			ok = true;
		}
	return ok;
	}

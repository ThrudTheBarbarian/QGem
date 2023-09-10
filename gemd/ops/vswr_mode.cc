#include <QPainter>

#include "clientmsg.h"
#include "debug.h"
#include "gem.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 32: Set the writing mode.
|*
|* Original signature is: vswr_mode(int16_t handle, int16_t mode);
|*
\*****************************************************************************/
void VDI::vswr_mode(Workstation *ws, ClientMsg *cm)
	{
	/**************************************************************************\
	|* Decode the incoming mode value
	\**************************************************************************/
	int16_t mode = cm->payload().at(0);
	if ((mode < WR_REPLACE) || (mode > WR_REV_TRANS))
		{
		WARN("Unrecognised mode (%d) in vswr_mode()", mode);
		}
	else switch (mode)
		{
		case WR_REPLACE:
			ws->setWrMode(QPainter::CompositionMode_Source);
			break;
		case WR_TRANSPARENT:
			ws->setWrMode(QPainter::CompositionMode_SourceOver);
			break;
		case WR_XOR:
			ws->setWrMode(QPainter::CompositionMode_Xor);
			break;
		case WR_REV_TRANS:	// Life is too short to figure out how this maps...
			ws->setWrMode(QPainter::CompositionMode_SourceOver);
			break;
		}
	}

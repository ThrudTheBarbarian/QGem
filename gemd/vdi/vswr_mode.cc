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
void VDI::vswr_mode(Workstation *ws, int16_t mode)
	{
	if ((mode < WR_REPLACE) || (mode > WR_REV_TRANS))
		{
		WARN("Unrecognised mode (%d) in vswr_mode()", mode);
		}
	else if (ws != nullptr)
		{
		ws->setWrMode(mode);
		}
	}

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
	int16_t mode = ntohs(cm->payload().at(0));
	vswr_mode(ws, mode);
	}

#include <QPainter>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 14: Set the RGB for a colour
|*
|* Original signature is: vs_color(int16_t handle, int16_t idx, int16_t rgb[3]);
|*
\*****************************************************************************/
void VDI::vs_color(qintptr handle, int16_t idx, int16_t *rgb)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((idx > 0) && (idx < 256))
		if (ws != nullptr)
			{
			for (int i=0; i<3; i++)
				rgb[i] = (rgb[i] < 0) ? 0
					   : (rgb[1] > 1000) ? 1000
					   : rgb[i];

			uint8_t R = (uint8_t)(((255 * rgb[0]) / 1000));
			uint8_t G = (uint8_t)(((255 * rgb[1]) / 1000));
			uint8_t B = (uint8_t)(((255 * rgb[2]) / 1000));
			ws->setColour(idx, R, G, B);
			}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vs_color(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t idx			= ntohs(p[0]);

	int16_t rgb[3];
	for (int i=0; i<3; i++)
		rgb[i] = ntohs(p[i+1]);

	vs_color(ws->handle(), idx, rgb);
	}

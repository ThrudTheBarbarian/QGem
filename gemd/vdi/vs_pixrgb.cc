#include <QColor>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 59	Set a pixel's RGB
|*
|* Original signature is: vs_pixrgb(int16_t handle, int16_t x, int16_t y,
|*									  int16_t* rgb);
|*
\*****************************************************************************/
void VDI::vs_pixrgb(qintptr handle, int16_t x, int16_t y, int16_t *rgb)
	{
	(void)handle;

	QColor c = QColor(rgb[0], rgb[1], rgb[2]);
	_img->setPixelColor(x, y, c);
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vs_pixrgb(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);

	int16_t rgb[3];
	for (int i=0; i<3; i++)
		rgb[i] = ntohs(p[i+2]);

	vs_pixrgb(ws->handle(), x, y, rgb);
	}




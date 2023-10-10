#include <QColor>

#include "clientmsg.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Opcode 60	Get a pixel's RGB
|*
|* Original signature is: vq_pixrgb(int16_t handle, int16_t x, int16_t y,
|*									int16_t* rgb;
|*
\*****************************************************************************/
void VDI::vq_pixrgb(qintptr handle, int16_t x, int16_t y, int16_t *rgb)
	{
	(void)handle;

	QRgb pix = _img->pixel(x, y);

	rgb[0] = qRed(pix);
	rgb[1] = qGreen(pix);
	rgb[2] = qBlue(pix);
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vq_pixrgb(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t x			= ntohs(p[0]);
	int16_t y			= ntohs(p[1]);

	int16_t rgb[3];
	vq_pixrgb(ws->handle(), x, y, rgb);

	/**************************************************************************\
	|* Construct the message
	\**************************************************************************/
	cm->clear();
	cm->append(rgb, 3);
	cm->setType(MSG_REPLY(ClientMsg::VQ_PIXRGB));

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}




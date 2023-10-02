
#include "clientmsg.h"
#include "connectionmgr.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"


/*****************************************************************************\
|* Foreard declarations...
\*****************************************************************************/
void _copyBitmap(uint16_t *dst, int wpl, uint16_t *src);

/*****************************************************************************\
|* Opcode 111 : Set the mouse form
|*
|* Original signature is: vsc_form(int16_t handle, int16_t *form);
|*
|* pform[0]	X-coordinate, and
|* pform[1]	Y-coordinate of hot-spot
|* pform[2]	Number of planes, must be 1
|* pform[3]	Colour index of background mask (should be 0)
|* pform[4]	Colour index of foreground mask (should be 1)
|* pform[05..20]	Definition of background mask
|* pform[21..36]	Definition of foreground mask
\*****************************************************************************/
void VDI::vsc_form(qintptr handle, int16_t *form)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	if ((ws != nullptr) && (form != nullptr))
		{
		QImage imgmask(16, 16, QImage::Format_Mono);
		_copyBitmap((uint16_t *)(imgmask.bits()),
					imgmask.bytesPerLine()/2,
					(uint16_t *)&(form[5]));
		QBitmap mask	= QBitmap::fromImage(imgmask);

		QPainter p(_img);
		p.drawPixmap(600, 20, 64, 64, mask);

		QImage img(16, 16, QImage::Format_Mono);
		_copyBitmap((uint16_t *)(img.bits()),
					img.bytesPerLine()/2,
					(uint16_t *)&(form[21]));
		QBitmap pix		= QBitmap::fromImage(img);
		p.drawPixmap(664, 20, 64, 64, pix);

		QCursor cursor	= QCursor(pix, mask, form[0], form[1]);
		_screen->setCursor(cursor);
		}
	}

/*****************************************************************************\
|* And from the socket interface...
\*****************************************************************************/
void VDI::vsc_form(Workstation *ws, ClientMsg *cm)
	{
	int16_t data[37];
	const Payload &p	= cm->payload();
	for (int i=0; i<5; i++)
		data[i] = ntohs(p[i]);
	memcpy(&(data[5]), &(p[5]), 64);

	vsc_form(ws->handle(), data);

	/**************************************************************************\
	|* Send the message down the wire
	\**************************************************************************/
	ws->send(cm);
	}

/*****************************************************************************\
|* Copy the bitmap, respecting the bytes-per-line
\*****************************************************************************/
void _copyBitmap(uint16_t *dst, int wpl, uint16_t *src)
	{
	for (int h=0; h<16; h++)
		{
		*dst = *src++;
		dst += wpl;
		}
	}

#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "fillfactory.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Opcode 112: Set the user-defined fill pattern
|*
|* Original signature is:
|*
|* void vsf_udpat(int16_t handle, int16_t *pat, int16_t planes)
|*
\*****************************************************************************/
void VDI::vsf_udpat(qintptr handle, int16_t *pat, int16_t planes)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	/*********************************************************************\
	|* Convert the 16x16 planes into chunky form
	\*********************************************************************/
	uint8_t pixels[16*16];
	uint16_t *ptr[planes];
	for (int i=0; i<planes; i++)
		ptr[i] = (uint16_t *)(pat + i * 16);

	int idx	  = 0;
	int words = 16;
	for (int word = 0; word < words; word ++)
		{
		uint16_t values[planes];
		for (int j=0; j<planes; j++)
			values[j] = *ptr[j] ++;

		int count = 16 / planes;
		int mask = 0x8000;
		for (int j=0; j<count; j++)
			{
			int pixel		= 0;
			int bitValue	= 1 << (planes-1);
			for (int k=0; k<planes; k++)
				{
				pixel += ((values[k] & mask) ? bitValue : 0);
				bitValue >>= 1;
				}
			mask >>= 1;

			pixels[idx++] = pixel;
			}
		}

	/*********************************************************************\
	|* Form an MFDB describing the pattern
	\*********************************************************************/
	MFDB info;
	info.fd_addr		= pixels;
	info.fd_nplanes		= planes;
	info.fd_h			= 16;
	info.fd_w			= 16;
	info.fd_wdwidth		= planes;
	info.fd_stand		= MFDB_DEVICE;

	/*********************************************************************\
	|* Check to see if either MFDB points to the screen
	\*********************************************************************/
	FillFactory& ff = FillFactory::sharedInstance();
	ff.setUserFill(&info, ws);
	}



/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vsf_udpat(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int16_t planes		= ntohs(p[0]);

	vsf_udpat(ws->handle(), (int16_t *)(&(p[1])), planes);
	}

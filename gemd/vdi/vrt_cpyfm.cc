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
|* Opcode 109	Perform a blit
|*
|* Original signature is:
|*
|*  void vrt_cpyfm (int16_t handle,
|*  				int16_t mode,
|*  				int16_t *pxy8,
|*  				MFDB *src,
|*  				MFDB *dst,
|*  				int16_t *colourIndex)
|*
|* Mode is specified as:
|*
|*  1: Replace				- CompositionMode_Source
|*  2: Transparent			- CompositionMode_SourceOver
|*  3: XOR					- RasterOp_NotSourceXorDestination
|*  4: Reverse Transparent	- RasterOp_NotSourceAndDestination
|*
\*****************************************************************************/
void VDI::vrt_cpyfm(qintptr handle, int16_t mode, int16_t *pxy,
					MFDB *src, MFDB *dst, int16_t *colours)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QPixmap srcP;
	QImage *dstP = nullptr;


	if (IS_OK(ws) && IS_OK(src) && IS_OK(dst) && IS_OK(pxy) && IS_OK(colours))
		{
		/*********************************************************************\
		|* Check to see if the MFDB points to the screen
		\*********************************************************************/
		FillFactory& ff = FillFactory::sharedInstance();
		dstP = (dst->fd_addr == 0) ? _img : ff.imageFromMFDB(dst, ws);

		/*********************************************************************\
		|* Set up the painter
		\*********************************************************************/
		QPainter painter(dstP);
		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		Palette palette;
		QPainter::CompositionMode op;
		switch (mode & 0x7FFF)
			{
			case WR_REPLACE:
				palette << ws->colour(colours[0]).rgba()
						<< ws->colour(colours[1]).rgba();
				op = QPainter::CompositionMode_Source;
				break;
			case WR_TRANSPARENT:
				{
				if (colours[1] == 0)
					palette << QColor(Qt::transparent).rgba()
							<< ws->colour(colours[0]).rgba();

				else
					palette << ws->colour(colours[1]).rgba()
							<< ws->colour(colours[0]).rgba();

				op = QPainter::CompositionMode_SourceOver;
				break;
				}
			case WR_XOR:
				palette << ws->colour(colours[0]).rgba()
						<< ws->colour(colours[1]).rgba();
				op = QPainter::RasterOp_NotSourceXorDestination;
				break;
			case WR_REV_TRANS:
				palette << ws->colour(colours[0]).rgba()
						<< ws->colour(colours[1]).rgba();
				op = QPainter::RasterOp_NotSourceAndDestination;
				break;
			default:
				palette << ws->colour(colours[0]).rgba()
						<< ws->colour(colours[1]).rgba();
				op = QPainter::CompositionMode_Source;
				WARN("Unknown raster operation %0x", mode & 0x7FFF);
				break;
			}
		painter.setCompositionMode(op);


		/*********************************************************************\
		|* Extract the X1,Y1 -> X2,Y2 from the pxy arrays
		\*********************************************************************/
		bool scale = (mode & 0x8000) ? true : false;
		QRect srcR(pxy[0], pxy[1], pxy[2] - pxy[0], pxy[3] - pxy[1]);
		QRect dstR(pxy[4], pxy[5], pxy[6] - pxy[4], pxy[7] - pxy[5]);

		/*********************************************************************\
		|* Blit from one to the other
		\*********************************************************************/
		srcP = ff.bitmapFromMFDB(src, palette);
		if (scale)
			painter.drawPixmap(dstR, srcP, srcR);
		else
			painter.drawPixmap(dstR.topLeft(), srcP, srcR);

		/*********************************************************************\
		|* Tidy up
		\*********************************************************************/
		if ((dstP != _img) && (dstP != nullptr))
			delete dstP;
		}
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vrt_cpyfm(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int idx				= 0;
	int16_t mode		= ntohs(p[idx++]);

	/*************************************************************************\
	|* Get the colour indexes
	\*************************************************************************/
	int16_t colourIndexes[2];
	colourIndexes[0] = ntohs(p[idx++]);
	colourIndexes[1] = ntohs(p[idx++]);

	/*************************************************************************\
	|* Set up the drawing rects
	\*************************************************************************/
	int16_t pxy[8];
	for (int i=0; i<8; i++)
		pxy[i] = ntohs(p[idx++]);


	/*************************************************************************\
	|* Set up the source MFDB
	\*************************************************************************/
	MFDB src;
	src.fd_addr			= 0;
	int fd_addr			= ntohs(p[idx++]);
	src.fd_w			= ntohs(p[idx++]);
	src.fd_h			= ntohs(p[idx++]);
	src.fd_wdwidth		= ntohs(p[idx++]);
	src.fd_stand		= ntohs(p[idx++]);
	src.fd_nplanes		= ntohs(p[idx++]);
	src.fd_r1			= ntohs(p[idx++]);
	src.fd_r2			= ntohs(p[idx++]);
	src.fd_r3			= ntohs(p[idx++]);

	if (src.fd_stand != MFDB_DEVICE)
		{
		WARN("Cannot copy src MFDB in standard form. Need to vr_trnfm()");
		return;
		}

	if (fd_addr != 0)
		{
		src.fd_addr		= new uint8_t[src.fd_wdwidth * 2 * src.fd_h];

		QByteArray ba;
		idx += cm->fetchData(idx, ba);
		if (src.fd_wdwidth * 2 * src.fd_h != ba.size())
			WARN("Mismatch in size within MFDB (%d != %d)",
				 src.fd_wdwidth * 2 * src.fd_h,
				 ba.size());

		memcpy(src.fd_addr, ba.constData(), ba.size());
		}

	/*************************************************************************\
	|* Set up the destination MFDB
	\*************************************************************************/
	MFDB dst;
	dst.fd_addr			= 0;
	fd_addr				= ntohs(p[idx++]);
	dst.fd_w			= ntohs(p[idx++]);
	dst.fd_h			= ntohs(p[idx++]);
	dst.fd_wdwidth		= ntohs(p[idx++]);
	dst.fd_stand		= ntohs(p[idx++]);
	dst.fd_nplanes		= ntohs(p[idx++]);
	dst.fd_r1			= ntohs(p[idx++]);
	dst.fd_r2			= ntohs(p[idx++]);
	dst.fd_r3			= ntohs(p[idx++]);


	if (dst.fd_stand != 0)
		{
		WARN("Cannot copy dst MFDB in standard form. Need to vr_trnfm()");
		return;
		}

	if (fd_addr != 0)
		{
		dst.fd_addr		= new uint8_t[dst.fd_wdwidth * 2 * dst.fd_h];

		QByteArray ba;
		cm->fetchData(idx, ba);
		if (dst.fd_wdwidth * 2 * src.fd_h != ba.size())
			WARN("Mismatch in size within dst MFDB (%d != %d)",
				 dst.fd_wdwidth * 2 * dst.fd_h,
				 ba.size());
		memcpy(dst.fd_addr, ba.constData(), ba.size());
		}

	/*************************************************************************\
	|* Call the actual drawing code, now that we've parsed the data
	\*************************************************************************/
	vrt_cpyfm(ws->handle(), mode, pxy, &src, &dst, colourIndexes);

	/*************************************************************************\
	|* And tidy up
	\*************************************************************************/
	if (src.fd_addr != 0)
		delete [] ((uint8_t *)(src.fd_addr));
	if (dst.fd_addr != 0)
		delete [] ((uint8_t *)(dst.fd_addr));


	/*************************************************************************\
	|* FIXME: If dst is not the screen, we ought to send it back
	\*************************************************************************/

	}

#pragma mark - Helper functions



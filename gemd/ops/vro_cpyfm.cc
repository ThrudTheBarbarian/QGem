#include <QColor>
#include <QPainter>
#include <QPainterPath>

#include "clientmsg.h"
#include "connectionmgr.h"
#include "macros.h"
#include "screen.h"
#include "vdi.h"
#include "workstation.h"

/*****************************************************************************\
|* Forward references
\*****************************************************************************/
static QImage * _imageFromMFDB(MFDB* mfdb);

/*****************************************************************************\
|* Opcode 109	Perform a blit
|*
|* Original signature is:
|*
|* void vro_cpyfm(qintptr handle, int16_t mode, int16_t *pxy,
|*				  MFDB *src, MFDB *dst);
|*
|* Mode is specified as:
|*
|*  0: ALL_WHITE	- RasterOp_ClearDestination
|*  1: S_AND_D		- RasterOp_SourceAndDestination
|*  2: S_AND_NOTD	- RasterOp_NotSourceAndNotDestination
|*  3: S_ONLY		- CompositionMode_Source
|*  4: NOTS_AND_D	- RasterOp_NotSourceAndDestination
|*  5: D_ONLY		- nop
|*  6: S_XOR_D		- RasterOp_SourceXorDestination
|*  7: S_OR_D		- RasterOp_SourceOrDestination
|*  8: NOT_SORD		- RasterOp_NotSourceOrDestination
|*  9: NOT_SXORD	- RasterOp_NotSourceXorDestination
|* 10: D_INVERT		- RasterOp_NotDestination
|* 11: NOT_D		- RasterOp_SourceOrNotDestination
|* 12: S_OR_NOTD	- RasterOp_NotSource
|* 13: NOTS_OR_D	- RasterOp_NotSourceOrDestination
|* 14: NOT_SANDD	- RasterOp_NotSourceAndNotDestination
|* 15: ALL_BLACK	- RasterOp_SetDestination
|*
\*****************************************************************************/
void VDI::vro_cpyfm(qintptr handle, int16_t mode, int16_t *pxy,
					MFDB *src, MFDB *dst)
	{
	ConnectionMgr *cm = _screen->connectionManager();
	Workstation *ws   = cm->findWorkstationForHandle(handle);

	QImage *srcP;
	QImage *dstP;

	if (IS_OK(ws) && IS_OK(src) && IS_OK(dst) && IS_OK(pxy))
		{
		/*********************************************************************\
		|* Extract the X1,Y1 -> X2,Y2 from the pxy arrays
		\*********************************************************************/
		bool scale = (mode & 0x8000) ? true : false;
		QRect srcR(pxy[0], pxy[1], pxy[2] - pxy[0], pxy[3] - pxy[1]);
		QRect dstR(pxy[4], pxy[5], pxy[6] - pxy[4], pxy[7] - pxy[5]);

		/*********************************************************************\
		|* Check to see if either MFDB points to the screen
		\*********************************************************************/
		srcP = (src->fd_addr == 0) ? _img : _imageFromMFDB(src);
		dstP = (dst->fd_addr == 0) ? _img : _imageFromMFDB(dst);

		QPainter painter(dstP);
		if (ws->enableClip())
			painter.setClipRect(ws->clip());

		/*********************************************************************\
		|* Set up the writing mode
		\*********************************************************************/
		QPainter::CompositionMode op;
		switch (mode & 0x7FFF)
			{
			case ALL_WHITE:
				op = QPainter::RasterOp_SetDestination;
				break;
			case S_AND_D:
				op = QPainter::RasterOp_SourceAndDestination;
				break;
			case S_AND_NOTD:
				op = QPainter::RasterOp_NotSourceAndNotDestination;
				break;
			case S_ONLY:
				op = QPainter::CompositionMode_Source;
				break;
			case NOTS_AND_D:
				op = QPainter::RasterOp_NotSourceAndDestination;
				break;
			case D_ONLY:
				op = QPainter::CompositionMode_Destination;
				break;
			case S_XOR_D:
				op = QPainter::RasterOp_SourceXorDestination;
				break;
			case S_OR_D:
				op = QPainter::RasterOp_SourceOrDestination;
				break;
			case NOT_SORD:
				op = QPainter::RasterOp_NotSourceOrDestination;
				break;
			case NOT_SXORD:
				op = QPainter::RasterOp_NotSourceXorDestination;
				break;
			case D_INVERT:
				op = QPainter::RasterOp_NotDestination;
				break;
			case NOT_D:
				op = QPainter::RasterOp_SourceOrNotDestination;
				break;
			case S_OR_NOTD:
				op = QPainter::RasterOp_NotSource;
				break;
			case NOTS_OR_D:
				op = QPainter::RasterOp_NotSourceOrDestination;
				break;
			case NOT_SANDD:
				op = QPainter::RasterOp_NotSourceAndNotDestination;
				break;
			case ALL_BLACK:
				op = QPainter::RasterOp_ClearDestination;
				break;
			default:
				op = QPainter::CompositionMode_Source;
				WARN("Unknown raster operation %0x", mode & 0x7FFF);
				break;
			}
		painter.setCompositionMode(op);


		/*********************************************************************\
		|* Blit from one to the other
		\*********************************************************************/
		if (scale)
			painter.drawImage(dstR, *srcP, srcR);
		else
			painter.drawImage(dstR.topLeft(), *srcP, srcR);

		/*********************************************************************\
		|* Tidy up
		\*********************************************************************/
		if (srcP != _img)
			delete srcP;
		if (dstP != _img)
			delete dstP;
		}
	}

/*****************************************************************************\
|* And from the socket interface
\*****************************************************************************/
void VDI::vro_cpyfm(Workstation *ws, ClientMsg *cm)
	{
	const Payload &p	= cm->payload();
	int idx				= 0;
	int16_t mode		= ntohs(p[idx++]);

	int16_t pxy[8];
	for (int i=0; i<8; i++)
		pxy[i] = ntohs(p[idx++]);

	MFDB src;
	src.fd_addr			= 0;
	int fd_addr			= ntohs(p[idx++]);
	src.fd_w			= ntohs(p[idx++]);
	src.fd_h			= ntohs(p[idx++]);
	src.fd_wdwidth		= ntohs(p[idx++]);
	src.fd_stand		= ntohs(p[idx++]);
	src.fd_r1			= ntohs(p[idx++]);
	src.fd_r2			= ntohs(p[idx++]);
	src.fd_r3			= ntohs(p[idx++]);

	if (src.fd_stand != 0)
		{
		WARN("Cannot copy src MFDB in standard form. Need to vr_trnfm()");
		return;
		}

	if (fd_addr != 0)
		{
		src.fd_addr		= new uint8_t[src.fd_wdwidth * 2 * src.fd_h];

		QByteArray ba;
		idx += cm->fetchData(idx, ba);
		memcpy(src.fd_addr, ba.constData(), ba.size());
		}

	MFDB dst;
	dst.fd_addr			= 0;
	fd_addr				= ntohs(p[idx++]);
	dst.fd_w			= ntohs(p[idx++]);
	dst.fd_h			= ntohs(p[idx++]);
	dst.fd_wdwidth		= ntohs(p[idx++]);
	dst.fd_stand		= ntohs(p[idx++]);
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
		memcpy(dst.fd_addr, ba.constData(), ba.size());
		}

	vro_cpyfm(ws->handle(), mode, pxy, &src, &dst);
	}

#pragma mark - Helper functions

/*****************************************************************************\
|* Create an image from the MFDB
\*****************************************************************************/
static QImage * _imageFromMFDB(MFDB *mfdb)
	{
	QImage *img = nullptr;
	if (IS_OK(mfdb))
		{
		QImage::Format fmt;
		switch (mfdb->fd_nplanes)
			{
			default:
				fmt = QImage::Format_Mono;
				break;
			case 8:
				fmt = QImage::Format_Indexed8;
				break;
			case 16:
				fmt = QImage::Format_RGB16;
				break;
			case 24:
				fmt = QImage::Format_RGB888;
				break;
			case 32:
				fmt = QImage::Format_RGB32;
				break;
			}

		img = new QImage((uchar *)mfdb->fd_addr,
						 mfdb->fd_w,
						 mfdb->fd_h,
						 mfdb->fd_wdwidth*2,
						 fmt);
		}
	return img;
	}


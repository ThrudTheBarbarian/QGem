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
static QImage * _imageFromMFDB(MFDB* mfdb, Workstation *ws);

/*****************************************************************************\
|* Colour index conversion
\*****************************************************************************/
static const short _devtovdi8[256] =
	{
	0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11,14,12,15,13,255,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,1
  };

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

	QImage *srcP = nullptr;
	QImage *dstP = nullptr;


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
		srcP = (src->fd_addr == 0) ? _img : _imageFromMFDB(src, ws);
		dstP = (dst->fd_addr == 0) ? _img : _imageFromMFDB(dst, ws);

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
		if ((srcP != _img) && (srcP != nullptr))
			delete srcP;
		if ((dstP != _img) && (dstP != nullptr))
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
	vro_cpyfm(ws->handle(), mode, pxy, &src, &dst);

	/*************************************************************************\
	|* And tidy up
	\*************************************************************************/
	if (src.fd_addr != 0)
		delete [] ((uint8_t *)(src.fd_addr));
	if (dst.fd_addr != 0)
		delete [] ((uint8_t *)(dst.fd_addr));
	}

#pragma mark - Helper functions

/*****************************************************************************\
|* Create an image from a 1-bit MFDB
\*****************************************************************************/
QImage * _imageFromMFDB1(MFDB *mfdb, Workstation *ws)
	{
	QList<QRgb> palette;

	uint16_t *ptr	= (uint16_t *)(mfdb->fd_addr);
	int numWords	= mfdb->fd_wdwidth * mfdb->fd_h;
	for (int i=0; i<numWords; i++)
		{
		*ptr = ntohs(*ptr);
		ptr ++;
		}

	QImage *img = new QImage((uchar *)mfdb->fd_addr,
							 mfdb->fd_w,
							 mfdb->fd_h,
							 mfdb->fd_wdwidth*2,
							 QImage::Format_Mono);
	if (ws->colourTable(palette))
		img->setColorTable(palette);

	return img;
	}

/*****************************************************************************\
|* Create an image from a 4-bit MFDB. We need to convert this to an 8-bit img
\*****************************************************************************/
QImage * _imageFromMFDB4(MFDB *mfdb, Workstation *ws)
	{
	QImage *img			= nullptr;
	int skip			= (mfdb->fd_w & 1) ? 1 : 0;
	int wds				= (mfdb->fd_w + skip)/2;
	uint8_t *imgData	= new uint8_t[wds * 2 * mfdb->fd_h];

	if (imgData)
		{
		int idx = 0;
		for (int i=0; i<mfdb->fd_h; i++)
			{
			uint8_t *data = ((uint8_t *)mfdb->fd_addr)
						  + i * mfdb->fd_wdwidth * 2;
			for (int j=0; j<mfdb->fd_w; )
				{
				uint8_t pixels	= *data ++;
				imgData[idx++]	= _devtovdi8[pixels >> 4];
				j++;
				if (j < mfdb->fd_w-1)
					{
					imgData[idx++]	= _devtovdi8[pixels & 0xf];
					j++;
					}
				}
			idx += skip;
			}

		/*********************************************************************\
		|* Remap the image with the 8-bit data
		\*********************************************************************/
		delete [] ((uint8_t *)(mfdb->fd_addr));
		mfdb->fd_addr		= imgData;
		mfdb->fd_nplanes	= 8;
		mfdb->fd_wdwidth	= wds;

		img = new QImage((uchar *)mfdb->fd_addr,
						 mfdb->fd_w,
						 mfdb->fd_h,
						 mfdb->fd_wdwidth*2,
						 QImage::Format_Indexed8);

		QList<QRgb> palette;
		if (ws->colourTable(palette))
			img->setColorTable(palette);
		}
	else
		WARN("Cannot allocate 8-bit image for 4-bit data in vro_cpyfm");

	return img;
	}

/*****************************************************************************\
|* Create an image from an 8-bit MFDB
\*****************************************************************************/
QImage * _imageFromMFDB8(MFDB *mfdb, Workstation *ws)
	{
	QList<QRgb> palette;
	QImage *img = new QImage((uchar *)mfdb->fd_addr,
							 mfdb->fd_w,
							 mfdb->fd_h,
							 mfdb->fd_wdwidth*2,
							 QImage::Format_Indexed8);
	if (ws->colourTable(palette))
		img->setColorTable(palette);

	return img;
	}


/*****************************************************************************\
|* Create an image from the MFDB
\*****************************************************************************/
static QImage * _imageFromMFDB(MFDB *mfdb, Workstation *ws)
	{
	QImage *img = nullptr;

	if (IS_OK(mfdb))
		{
		switch (mfdb->fd_nplanes)
			{
			default:
				img = _imageFromMFDB1(mfdb, ws);
				break;
			case 4:
				img = _imageFromMFDB4(mfdb, ws);
				break;
			case 8:
				img = _imageFromMFDB8(mfdb, ws);
				break;
			case 16:
				WARN("16-bit images currently not implemented");
				// FIXME: fmt = QImage::Format_RGB16;
				break;
			case 24:
				WARN("24-bit images currently not implemented");
				// FIXME: fmt = QImage::Format_RGB888;
				break;
			case 32:
				WARN("Unknown image depth of %d", mfdb->fd_nplanes);
				break;
			}
		}

	return img;
	}


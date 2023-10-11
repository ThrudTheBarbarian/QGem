#ifndef __gem_types_header__
#define __gem_types_header__

#include <stdint.h>

/*****************************************************************************\
|* Raster operations 
\*****************************************************************************/
typedef struct
	{
   	void *fd_addr;					/* Pointer to the start of the
                                    	memory block, e.g. the
                                    	screen memory base address  	*/
   	int16_t  fd_w;					/* Width in pixels             		*/
   	int16_t  fd_h;					/* Height in pixels            		*/
   	int16_t  fd_wdwidth;			/* Width of a line in words   		*/
   	int16_t  fd_stand;				/* 0 = Device-specific format  		*/
                                 	/* 1 = Standard format         		*/
   	int16_t  fd_nplanes;			/* Number of planes            		*/
   	int16_t  fd_r1, fd_r2, fd_r3;	/* Reserved, must be 0         		*/
	} MFDB;



/*****************************************************************************\
|* Typedefs
\*****************************************************************************/
#ifdef __cplusplus
	#include <vector>
	#include <QList>
	#include <QColor>		// For QRgb
	
	typedef std::vector<int16_t>	Payload;
	typedef std::vector<uint8_t>	ByteStream;
	typedef QList<qreal> 			LineStyle;
	typedef QList<qintptr>			HandleList;
	typedef QList<QRgb>				Palette;
#endif

#endif /* ! __gem_types_header__ */

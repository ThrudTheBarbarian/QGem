
#include "debug.h"
#include "vdi.h"

/*****************************************************************************\
|* Opcode 5.23: print image, which isn't supported
|*
|* Original signature is: v_bit_image(int16_t handle, const char *filename,
|*						 int16_t aspect, int16_t scaling, int16_t numPts,
|*						 int16_t *pts);
|*
\*****************************************************************************/
void VDI::v_bit_image(qintptr handle, const char *filename,
					  int16_t aspect, int16_t scaling, int16_t numPts,
					  int16_t *pts)
	{
	(void) filename;
	(void) aspect;
	(void) scaling;
	(void) numPts;
	(void) pts;

	if (handle == 0)
		{
		}
	else
		{
		WARN("Non-screen devices currently unsupported");
		}
	}

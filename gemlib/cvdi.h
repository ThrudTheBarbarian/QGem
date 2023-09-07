#ifndef CVDI_H
#define CVDI_H

/*****************************************************************************\
|* C interface headers for the VDI
\*****************************************************************************/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************\
|* Opcode 100	: Open a virtual workstation
\*****************************************************************************/

void v_opnvwk(int16_t *workIn, int16_t *handle, int16_t *workOut);


#ifdef __cplusplus
}
#endif

#endif // CVDI_H


#include <stdint.h>
#include "gemdefs.h"

int main(int argc, char *argv[])
	{
	(void)argc;
	(void)argv;

	int16_t workIn[15];
	int16_t handle;
	int16_t workOut[58];

	v_opnvwk(workIn, &handle, workOut);
	}

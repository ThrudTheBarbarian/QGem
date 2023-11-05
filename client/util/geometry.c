//
//  geometry.c
//  gemc
//
//  Created by Simon Gornall on 11/4/23.
//

#include "geometry.h"

static inline int32_t MAX(int32_t a, int32_t b) { return((a) > (b) ? a : b); }
static inline int32_t MIN(int32_t a, int32_t b) { return((a) < (b) ? a : b); }

/*****************************************************************************\
|* Intersection of two rectangles
\*****************************************************************************/
int intersection(Rect r1, Rect r2, Rect *result)
	{
	int ok		= 0;
	int leftX   = MAX(r1.x, r2.x);
	int rightX  = MIN(r1.x + r1.w, r2.x + r2.w);
	int topY    = MAX(r1.y, r2.y);
	int bottomY = MIN(r1.y + r1.h, r2.y + r2.h);
	
	if (leftX < rightX && topY < bottomY )
		{
		if (result)
			*result = (Rect){leftX, topY, rightX-leftX, bottomY-topY};
		ok = 1;
		}
		
	return ok;
	}

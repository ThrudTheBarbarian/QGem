//
//  macros.h
//  dbio
//
//  Created by Simon Gornall on 7/17/20.
//  Copyright © 2020 Simon Gornall. All rights reserved.
//

#ifndef macros_h
#define macros_h

#ifdef __cplusplus
  #include <vector>
  #include <QPen>
#endif

#include <stdlib.h>
#include "debug.h"
#include "gemTypes.h"

/*****************************************************************************\
|* These will release the memory they point to (from various allocators) and
|* then null out the pointer
\*****************************************************************************/
#define FREE(x)						do 								\
										{ 							\
										::free((void *)x); 			\
										x = nullptr; 				\
										} 							\
									while (false)

#define DELETE(x)					do 								\
										{ 							\
										delete x;			 		\
										x = nullptr; 				\
										} 							\
									while (false)

#define DELETE_ARRAY(x)				do 								\
										{ 							\
										delete [] x; 				\
										x = nullptr; 				\
										} 							\
									while (false)

#define DELETE_TYPED_ARRAY(x,y)		do 								\
										{ 							\
										delete [] (y*)(x); 			\
										x = nullptr; 				\
										} 							\
									while (false)

#define GPU_FREE(x)					do								\
										{							\
										GPU_FreeImage(x);			\
										x = nullptr;				\
										}							\
									while (false)

/*****************************************************************************\
|* Xcode provides these sometimes, depending on whether you include some magic
|* header file...
\*****************************************************************************/
#ifndef MIN
#	define MIN(x,y)  (((x) < (y)) ? (x) : (y))
#endif

#ifndef ABS
#	define ABS(x)    (((x) < 0) ? -(x) : (x))
#endif

/*****************************************************************************\
|* It's just *so* hard to leave ObjC behind...
\*****************************************************************************/
#ifndef YES
#   define YES true
#endif

#ifndef NO
#   define NO false
#endif

/*****************************************************************************\
|* Use RIAA to hold a lock
\*****************************************************************************/
#if THREADSAFE_COLLECTIONS
#  define GUARD	std::lock_guard<std::recursive_mutex> lock(_mutex)
#else
#  define GUARD
#endif

/*****************************************************************************\
|* Timing macro between TICK and TOCK. The 'stt' must be co-defined to be the
|* same variable
\*****************************************************************************/
#define TICK(stt)   struct timeval stt;                                     \
                    gettimeofday(&stt, NULL)                                \
                            
#define TOCK(stt,end,dt,r)                                                  \
                    struct timeval end,dt;                                  \
                    gettimeofday(&end, NULL);                               \
                    timersub(&end, &stt, &dt);                              \
                    double r = dt.tv_sec + ((double)(dt.tv_usec)/1000000)   \


/*****************************************************************************\
|* Create a rect
\*****************************************************************************/
#define MKRECT(x,y,w,h) 	{(float)(x), (float)(y), (float)(w), (float)(h)}


/*****************************************************************************\
|* Null test
\*****************************************************************************/
#define IS_OK(x) 	((x) != nullptr)

#endif /* macros_h */

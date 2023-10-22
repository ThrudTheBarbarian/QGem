//
//  debug.h
//  dbio
//
//  Created by ThrudTheBarbarian on 7/17/20.
//

#ifndef debug_h
#define debug_h

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

/*****************************************************************************\
|* Some level of cross-platform support
\*****************************************************************************/
#if defined(DEBUG)
#   if defined(_MSC_VER)
#       // "(void)0," is for suppressing C4127 warning in
#       // "assert(false)", "assert(true)" and the like
#       define DBG_ASSERT( x )       if ( !((void)0,(x))) { __debugbreak(); }
#                                    //if ( !(x)) WinDebugBreak()
#   else
#       include <assert.h>
#       define DBG_ASSERT            assert
#   endif
#else
#   define DBG_ASSERT( x )           {}
#endif

/*****************************************************************************\
|* The intent here is that logging should be done via macros, so that any
|* given logger can be plugged in by redefining the macro at some later
|* stage. The macros here just dump to stdout
\*****************************************************************************/

#ifdef __cplusplus

#define LOG(...) 													\
do 																\
		{ 															\
			fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);			\
			fprintf(stderr,  __VA_ARGS__);								\
			fprintf(stderr, "\n");										\
		} while (0)

#if defined(DEBUG)

extern int debugLevel(void);

#define WARN(...) 													\
do 																\
		{ 															\
			if (debugLevel() > 0) 										\
			{														\
				fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);		\
				fprintf(stderr,  __VA_ARGS__); 							\
				fprintf(stderr, "\n");									\
			} 														\
		} while (0)

#define INFO(...) 													\
		do 																\
		{ 															\
			if (debugLevel() > 1) 										\
			{														\
				fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);		\
				fprintf(stderr, __VA_ARGS__); 							\
				fprintf(stderr, "\n");									\
			} while (0)
#else
# define WARN(...)
# define INFO(...)
#endif // DEBUG defined

#else

#if defined(DEBUG)

#define LOG(...) 													\
	do 																\
		{ 															\
		fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);			\
		fprintf(stderr,  __VA_ARGS__);								\
		fprintf(stderr, "\n");										\
		} while (0)

#define WARN(...) 													\
	do 																\
		{ 															\
		if (debugLevel() > 0) 										\
			{														\
			fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);		\
			fprintf(stderr,  __VA_ARGS__); 							\
			fprintf(stderr, "\n");									\
			} 														\
		} while (0)

#define INFO(...) 													\
	do 																\
		{ 															\
		if (debugLevel() > 1) 										\
			{														\
			fprintf(stderr, "%s:%d ", __FILENAME__, __LINE__);		\
			fprintf(stderr, __VA_ARGS__); 							\
			fprintf(stderr, "\n");									\
		} while (0)

#else
# define LOG(...)
# define WARN(...)
# define INFO(...)
#endif // DEBUG defined

#endif

/*****************************************************************************\
|* Provide weakly-defined debugLevel call that can be overridden by code
\*****************************************************************************/
int __attribute__((weak)) debugLevel(void);


#endif /* debug_h */

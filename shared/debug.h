//
//  debug.h
//  dbio
//
//  Created by Simon Gornall on 7/17/20.
//  Copyright © 2020 Simon Gornall. All rights reserved.
//

#ifndef debug_h
#define debug_h

#include <iostream>
#include <cstdio>
#include <cstring>
#include <sys/time.h>

#ifndef __FILENAME__
#  define __FILENAME__ (strrchr(__FILE__, '/')                      \
                        ? strrchr(__FILE__, '/') + 1                \
                        : __FILE__)
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



# define LOG(...) 													\
	{																\
    char s_str[4096];												\
    snprintf(s_str, 4095,__VA_ARGS__);								\
    std::cout << "[" << __FILENAME__ << " "						    \
			  << __FUNCTION__ << ": " << __LINE__ << "] " 		    \
			  << s_str << std::endl;								\
    }

#if defined(DEBUG)

extern int debugLevel();

# define WARN(...)                                                  \
    {                                                               \
    if (debugLevel() > 0)                                           \
        LOG(__VA_ARGS__)                                            \
    }

# define INFO(...)                                                  \
    {                                                               \
    if (debugLevel() > 1)                                           \
        LOG(__VA_ARGS__)                                            \
    }
#else
# define WARN(...)
# define INFO(...)
#endif // DEBUG defined

/*****************************************************************************\
|* Provide weakly-defined debugLevel call that can be overridden by code
\*****************************************************************************/
int __attribute__((weak)) debugLevel(void);


#endif /* debug_h */

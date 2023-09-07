//
//  properties.h
//  iodb
//
//  Created by Simon Gornall on 2/23/22.
//

#ifndef properties_h
#define properties_h


#include <inttypes.h>

/*****************************************************************************\
|* Getter / Setter macros for properties within classes.
\*****************************************************************************/
#define GETSETP(type, var, accessor)					\
	protected:											\
	   type _##var;										\
	public:												\
       inline type var() const							\
	   {												\
		  return _##var;								\
	   }												\
       inline void set##accessor(const type val)		\
	   {												\
		  _##var = const_cast<type>(val);				\
	   }

#define GETSET(type, var, accessor)						\
	protected:											\
	   type _##var;										\
	public:												\
       inline const type& var() const					\
	   {												\
		  return _##var;								\
	   }												\
       inline void set##accessor(const type& val)		\
	   {												\
		  _##var = val;									\
	   }

#define SET(type, var, accessor)						\
	protected:											\
	   type _##var;										\
	public:												\
       inline void set##accessor(const type &val)		\
	   {												\
		  _##var = val;									\
	   }

#define GET(type, var)									\
	protected:											\
	   type _##var;										\
	public:												\
       inline const type& var() const					\
	   {												\
		  return _##var;								\
	   }

#define GETP(type, var)									\
	protected:											\
	   type _##var;										\
	public:												\
       inline type var() const							\
	   {												\
		  return _##var;								\
	   }

/*****************************************************************************\
|* Make a class non-copiable or moveable
\*****************************************************************************/
#define NON_COPYABLE_NOR_MOVEABLE(T)					\
	  T(T const &) = delete;							\
	  void operator=(T const &t) = delete;				\
	  T(T &&) = delete;

#include "structures.h"

#endif /* properties_h */

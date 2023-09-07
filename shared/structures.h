//
//  structures.h
//  dbio
//
//  Created by Simon Gornall on 7/18/20.
//  Copyright © 2020 Simon Gornall. All rights reserved.
//

#ifndef structures_h
#define structures_h

#include <map>

#include <stdint.h>

/*****************************************************************************\
|* Simple integer point
\*****************************************************************************/
typedef struct 
	{
	int x;
	int y;
	} Point2D;
	
/*****************************************************************************\
|* Basically a clone of NSRange
\*****************************************************************************/
typedef struct Range
	{
	uint64_t    location;		// first index
	int64_t     length;			// length of range
 
    Range(uint64_t offset, int64_t len)
        {
        location = offset;
        length   = len;
        }
    
    Range()
        {
        location    = 0;
        length      = 0;
        }
        
	} Range;

#define NotFound        ((uint64_t)-1)

/*****************************************************************************\
|* Used to iterate over a map using the for: approach, as in:
|*
|*  for (Elements<int, std::string> kv : my_map )
|*		{
|*		std::cout << kv.key << " --> " << kv.value;
|*		}
\*****************************************************************************/
template <class K, class T>
struct Elements {
    K const& key;
    T& value;
 
    Elements(std::pair<K const, T>& pair)
        : key(pair.first)
        , value(pair.second)
    { }
};

/*****************************************************************************\
|* Database blob format definitions
\*****************************************************************************/
enum
	{
	BLOB_PNG	= 0,
	BLOB_JPG,
	BLOB_RGBA,
	BLOB_GREEN,
	BLOB_RGB
	};
	
/*****************************************************************************\
|* Shared between Brush and UI
\*****************************************************************************/
enum
	{
	BRUSH_IN = 0,
	BRUSH_OUT,
	BRUSH_UNITY,
	BRUSH_ZERO,
	BRUSH_BITMAP
	};

enum
	{
	BITMAP_POINT = 0,
	BITMAP_RECT,
	BITMAP_OVAL
	};

/*****************************************************************************\
|* Shared between Layer and UI
\*****************************************************************************/
enum
	{
	MASK_ALL = 0,
	MASK_FUNC,
	MASK_BRUSH
	};

enum
	{
	LAYER_NONE = -1,
	LAYER_TEXTURE = 0,
	LAYER_STAMP,
	LAYER_BITMAP,
	LAYER_SCATTER
	};


#endif /* structures_h */

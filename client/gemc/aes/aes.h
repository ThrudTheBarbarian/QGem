//
//  aes.h
//  client
//
//  Created by Simon Gornall on 10/10/23.
//

#ifndef aes_h
#define aes_h


#include "gem.h"

/*****************************************************************************\
|*  6007 : appl_init()
|*		 : Registers the application under AES
\*****************************************************************************/
int16_t appl_init(void);


/*****************************************************************************\
|*  6902 : graf_handle()
|*		 : Retrieve the AES physical workstation id and char stats
\*****************************************************************************/
int16_t graf_handle(int16_t *wchr, int16_t *hchr, int16_t *wbox, int16_t *hbox);



/*****************************************************************************\
|*  7800 : rsrc_free()
|*		 : Free up the resources used by a resource file
\*****************************************************************************/
int16_t rsrc_free(void);

/*****************************************************************************\
|*  7801 : rsrc_gaddr()
|*		 : Obtain a pointer to various object structures in the resource
\*****************************************************************************/
int16_t rsrc_gaddr(int16_t type, int16_t index, void *address);

/*****************************************************************************\
|*  7802 : rsrc_load()
|*		 : Load up the RSC file for this application
\*****************************************************************************/
int16_t rsrc_load(const char *filename);

/*****************************************************************************\
|*  7803 : rsrc_obfix()
|*		 : Convert an object from character-based to pixel-based
\*****************************************************************************/
int16_t rsrc_obfix(OBJECT *tree, int16_t object);

#endif /* aes_h */

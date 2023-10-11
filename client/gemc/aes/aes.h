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

#endif /* aes_h */

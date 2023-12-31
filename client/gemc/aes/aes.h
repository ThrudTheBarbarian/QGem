//
//  aes.h
//  client
//
//  Created by ThrudTheBarbarian on 10/10/23.
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
|*  6604 : wind_create()
|*		 : Reserve a window structure on the server. The xywh are max sizes.
|*       : Returns the handle of the window created, or -1
\*****************************************************************************/
int16_t wind_create(int16_t kind, int16_t x, int16_t y, int16_t w, int16_t h);

/*****************************************************************************\
|*  6608 : wind_get()
|*		 : Obtain various pieces of info about a window, and return in the
|*       : gw* variables
\*****************************************************************************/
int16_t wind_get(int16_t windowId, int16_t sub,
				 int16_t* gw1, int16_t* gw2, int16_t* gw3, int16_t* gw4);

/*****************************************************************************\
|*  6610 : wind_open()
|*		 : Open a window to the specified size
\*****************************************************************************/
int16_t wind_open(int16_t handle, int16_t x, int16_t y, int16_t w, int16_t h);


/*****************************************************************************\
|*  7002 : graf_handle()
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

/*****************************************************************************\
|*  7805 : rsrc_saddr()
|*		 : Set the pointer for an object structure into the RSC file. This
|* 		 : will make the RSC file take ownership of the memory
\*****************************************************************************/
int16_t rsrc_saddr(int16_t type, int16_t index, void *address);

/*****************************************************************************\
|*  7900 : shel_envrn()
|*		 : obtains the value of an environment variable
\*****************************************************************************/
int16_t shel_envrn(char **value, char *varname);

/*****************************************************************************\
|*  7901 : shel_find()
|*		 : Search for a file in a variety of system dirs. Note that 'name'
|* 		 : on this system should be PATH_MAX bytes long, the original Atari
|*       : suggestion was 128, which is not sufficient
|*
|* 		 : Returns 0 if the file cannot be found
\*****************************************************************************/
int16_t shel_find(char *name);

/*****************************************************************************\
|*  7902 : shel_get()
|*		 : obtains the GEM desktop environment information
\*****************************************************************************/
int16_t shel_get(char *info, uint16_t maxBytes);

/*****************************************************************************\
|*  7903 : shel_put()
|*		 : stores the GEM desktop environment information
\*****************************************************************************/
int16_t shel_put(char *info, uint16_t maxBytes);

/*****************************************************************************\
|*  7906 : shel_read()
|*		 : Get the commandline parameters for an application launched with
|*       : shel_write()
|*
|* Note that both cmd and args ought to be of length at least PATH_MAX
\*****************************************************************************/
int16_t shel_read(char *cmd, char *args);

/*****************************************************************************\
|*  7908 : shel_write()
|*		 : Interface to executing an app, or sending messages to other apps
\*****************************************************************************/
int16_t shel_write(int16_t doex,
				   int16_t isgr,
				   int16_t iscr,
				   char *  cmd,
				   char *  args);

/*****************************************************************************\
|* Listen on the event pipe and only return when we get a GEM-message type
|* event (in the 18xxx range). We still populate the mouse position events etc.
\*****************************************************************************/
int16_t evnt_mesag(int16_t* msg);

#endif /* aes_h */

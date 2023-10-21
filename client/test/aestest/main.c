//
//  main.c
//  aestest
//
//  Created by ThrudTheBarbarian on 10/10/23.
//

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
  
#include "aes.h"
#include "macros.h"
#include "vdi.h"

#define RSCNAME "src/QT/QGem/client/test/aestest/test.rsc"

/*****************************************************************************\
|* For debugging
\*****************************************************************************/
int debugLevel(void)
	{ return 10; }
	

/*****************************************************************************\
|* Program entry
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	// insert code here...
	printf("Simple AES test harness\n");
	
	/*************************************************************************\
	|* Start things going
	\*************************************************************************/
	int16_t appId = appl_init();
	if (appId < 0)
		{
		WARN("Cannot connect to the GEM service");
		exit(-1);
		}
	else
		LOG("Got application id %d\n", appId);

	/*************************************************************************\
	|* Get the AES handle
	\*************************************************************************/
	int16_t handle = graf_handle(NULL, NULL, NULL, NULL);
	
	/*************************************************************************\
	|* Open a virtual workstation
	\*************************************************************************/
	int16_t workIn[12], workOut[57];
	for(int i=1; i<10; i++)
		workIn[i]	= 1;
	workIn[10]		= 2;
	
	v_opnvwk(workIn,&handle,workOut);
	
	/*************************************************************************\
	|* Load up the resource file (twice)
	\*************************************************************************/
#if 0
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "%s/%s", getenv("HOME"), RSCNAME);
	if(!rsrc_load(path))
		{
		//form_alert(1,"[1][The resource file is|missing!][Cancel]");
		WARN("The resource file is corrupt or missing!");
		v_clsvwk(handle);
		exit(1);
		}
	if(!rsrc_load(path))
		{
		//form_alert(1,"[1][The resource file is|missing!][Cancel]");
		WARN("The resource file is corrupt or missing!");
		v_clsvwk(handle);
		exit(1);
		}
#endif

	/*************************************************************************\
	|* See if we can find the PATH shell variable
	\*************************************************************************/
	char *pathVar = NULL;
	shel_envrn(&pathVar, "PATH=");
	fprintf(stderr, "PATH: '%s'\n", pathVar);
	shel_envrn(&pathVar, "PATH");
	fprintf(stderr, "PATH: '%s'\n", pathVar);


	/*************************************************************************\
	|* Locate a file
	\*************************************************************************/
	char locate[PATH_MAX];
	strcpy(locate, "ls");
	if (shel_find(locate))
		fprintf(stderr, "Found '%s'\n", locate);

	/*************************************************************************\
	|* Get the desktop config
	\*************************************************************************/
	char dcfg[8192];
	if (shel_get(dcfg, 8192))
		fprintf(stderr, "CFG\n'%s'\n", dcfg);
		
	strcat(dcfg, "\nBoom!\n");
	shel_put(dcfg, strlen(dcfg));
	
	return 0;
	}

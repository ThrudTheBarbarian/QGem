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

#define RSCNAME "src/QT/QGem/client/aestest/test.rsc"

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
	|* Load up the resource file
	\*************************************************************************/
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "%s/%s", getenv("HOME"), RSCNAME);
	if(!rsrc_load(path))
		{
		//form_alert(1,"[1][The resource file is|missing!][Cancel]");
		WARN("The resource file is missing!");
		v_clsvwk(handle);
		exit(1);
		}
	return 0;
	}

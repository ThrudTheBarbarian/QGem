//
//  main.c
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#include <stdio.h>

#include "args.h"
#include "listing.h"
#include "match.h"
#include "rscfile.h"
#include "vec.h"

/*****************************************************************************\
|* Commandline options stuff
\*****************************************************************************/
static const char *const usages[] =
	{
    "rscdump [options] [[--] args]",
    "rscdump [options]",
    NULL,
	};

static const char *prefix 	= "\nDump out the structure of an RSC file";
static const char *postfix	= "\nEnjoy.";
static int _debugLevel		= 10;

/*****************************************************************************\
|* Tool to dump out an RSC file so we can play with the innards
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	const char *path 		= NULL;
	int verbose		 		= 0;
	int export		 		= 0;
	int listToStdout		= 0;
	const char *criterion 	= NULL;
	
	/*************************************************************************\
	|* Define the command-line options...
	\*************************************************************************/
    struct argparse_option options[] =
		{
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('f', "file", &path, "path to RSC file", NULL, 0, 0),
        OPT_BOOLEAN('v', "verbose", &verbose, "Be chatty", NULL, 0, 0),
        OPT_BOOLEAN('d', "debug", &_debugLevel, "Debugging flag", NULL, 0, 0),
        OPT_GROUP("Filtering options"),
        OPT_STRING('m', "match", &criterion, "substring match for export", NULL, 0, 0),
        OPT_GROUP("Output options"),
        OPT_BOOLEAN('e', "export", &export, "Export to files", NULL, 0, 0),
        OPT_BOOLEAN('l', "list", &listToStdout, "List to stdout", NULL, 0, 0),
        OPT_END(),
		};

	/*************************************************************************\
	|* ... and extract them
	\*************************************************************************/
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, prefix, postfix);
    argc = argparse_parse(&argparse, argc, argv);

	/*************************************************************************\
	|* List of object-ids to show
	\*************************************************************************/
	vec_int_t objIds;
	vec_init(&objIds);

	if (path != NULL)
		{
		RscFile rsc;
		resourceLoad(path, &rsc);
		if (verbose)
			{
			printf("RSC file size          : %u bytes\n", rsc.size);
			printf("RSC contains           : %d colour icons\n", rsc.nCicons);
			}
		
		/*********************************************************************\
		|* Preselect all objects by default
		\*********************************************************************/
		for (int i=0; i<rsc.nObjects; i++)
			vec_push(&objIds, i);
			
		/*********************************************************************\
		|* Run through the object tree and see if anything matches
		\*********************************************************************/
		if (criterion != NULL)
			{
			vec_clear(&objIds);
			for (int i=0; i<rsc.nObjects; i++)
				if (match(&(rsc.objects[i]), criterion))
					vec_push(&objIds, i);
			}
		
		/*********************************************************************\
		|* Do any listing
		\*********************************************************************/
		if (listToStdout)
			{
			for (int i=0; i<objIds.length; i++)
				listObject(&(rsc.objects[objIds.data[i]]));
			}
		}
	}


int debugLevel(void)
	{ return _debugLevel; }

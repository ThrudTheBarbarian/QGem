//
//  main.c
//  rscdump
//
//  Created by ThrudTheBarbarian on 9/22/23.
//

#include <stdio.h>

#include "args.h"
#include "rscfile.h"

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

/*****************************************************************************\
|* Tool to dump out an RSC file so we can play with the innards
\*****************************************************************************/
int main(int argc, const char * argv[])
	{
	const char *path = NULL;
	int verbose		 = 0;
	
	/*************************************************************************\
	|* Define the command-line options...
	\*************************************************************************/
    struct argparse_option options[] =
		{
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('f', "file", &path, "path to RSC file", NULL, 0, 0),
        OPT_BOOLEAN('v', "verbose", &verbose, "Be chatty", NULL, 0, 0),
        OPT_END(),
		};

	/*************************************************************************\
	|* ... and extract them
	\*************************************************************************/
    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, prefix, postfix);
    argc = argparse_parse(&argparse, argc, argv);
	
	if (path != NULL)
		{
		RscFile rsc;
		resourceLoad(path, &rsc);
		if (verbose)
			{
			printf("RSC file size          : %u bytes\n", rsc.size);
			printf("RSC contains           : %d colour icons\n", rsc.nCicons);
			}
		}
	}

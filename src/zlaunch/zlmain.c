/* zlmain - utility for launching zzt for zzt worlds in any directory */
/* $id$ */
/* Copyright (c) 2001 Ryan Phillips <bitman@users.sf.net> */

#include "zlaunch.h"
#include "files.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>   /* For chdir() */
#include <string.h>


void
launch(char* datapath, stringvector files, stringvector options,
			 stringvector info)
{
	zlaunchinfo zli = loadzlinfofromsvector(info);

	/* Configure */
	zli.datadir = str_dup(datapath);
	zli.bindir  = str_dup(datapath);
	/* Merge the lists. We are no longer responible for them. */
	zli.paramlist = files;
	stringvectorcat(&(zli.actionstoperform), &options);

	/* Try it */
	zlaunchact(&zli);

	/* Cleanup */
	zlaunchcleanup(&zli);
	deletezlinfo(&zli);
}


int
main(int argc, char* argv[])
{
	char* datapath  = NULL;
	char* worldpath = NULL;
	stringvector files, options;
	stringvector info;
	int i;

	initstringvector(&files);
	initstringvector(&options);

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '\0') {
			if (argv[i][0] == '-') {
				/* Options go into one list */
				int pos = 1;
				if (argv[i][pos] == '-') pos++;
				pushstring(&options, str_dup(argv[i] + pos));
			} else if (worldpath == NULL) {
				/* The path of the first non-option is extracted */
				char* worldfile;
				worldpath = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				worldfile = (char*) malloc(sizeof(char) * (strlen(argv[i]) + 1));
				pathof(worldpath, argv[i], strlen(argv[i]));
				fileof(worldfile, argv[i], strlen(argv[i]));
				pushstring(&files, worldfile);
			} else {
				/* All other non-options are added as files verbatum */
				pushstring(&files, str_dup(argv[i]));
			}
		}
	}

	if (worldpath == NULL) {
		printf("zlaunch " zlaunchVERSION "\n"
					 "Copyright (c) 2001  Ryan Phillips <bitman@users.sf.net>\n"
					 "This program may be freely redistributed under the terms of the "
					 "GNU GPL\n\n"
					 
					 "Usage: zlaunch path\\world.zzt\n"
		       "  zlaunch cleanly runs zzt for the given world, after switching\n"
					 "  to the directory of that world and copying zzt.dat there.\n\n"

					 "zlaunch MUST be in the same directory as zzt!\n");

		return 1;
	}

	/* Switch into the directory of the given zzt world */
	if (chdir(worldpath)) {
		/* Failed to change directory! */
		perror(worldpath);

		deletestringvector(&files);
		deletestringvector(&options);
		free(worldpath);
		return 1;
	}

	datapath  = locateself(argv[0]);

	info = loadinfo(datapath, files.first->s);

	launch(datapath, files, options, info);

	deletestringvector(&info);
	free(worldpath);
	free(datapath);
	return 0;
}



// Copyright (C) 2009 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

/**
 * \file
 * \brief  execpermfix main function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "execpermfix.h"

#define PROGRAM_NAME "execpermfix"
#define PROGRAM_VERSION "1.0.7"

/****************************************************************************/

const char USAGE[] =
    "Usage: execpermfix [-v] [-h] <paths...>\n"
    "Fixes executable permission of files based on the file type and on the\n"
    "current read permissions.\n\n"
    "    -h    This help.\n"
    "    -V    Version information.\n"
    "    -v    Verbose mode.\n"
    "    -n    Performs a trial run, no changes are made.\n";

/****************************************************************************/

int main(int argc, char *argv[]) {
    int rv = 0;
    int i;
    int opt;
    const char options[] = "hvVn";
    int verbose = 0;
    int noop = 0;

    while ((opt = getopt(argc, argv, options)) != -1) {
        switch (opt) {
            case 'h':
                printf("%s", USAGE);
                return 0;
            case 'v':
                verbose = 1;
                break;
            case 'V':
                printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
                exit(0);
                break;
            case 'n':
                noop = 1;
                break;
            case '?':
                fprintf(stderr, "Unknown option/command.\n%s", USAGE);
                return 1;
            default:
                fprintf(stderr, "Error: option/command not found!\n%s", USAGE);
                return 1;
        }
    }

    for (i = optind; i < argc; i++) rv |= execpermfix(argv[i], noop, verbose);

    return rv;
}

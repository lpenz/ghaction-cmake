// Copyright (C) 2009 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

/**
 * \file
 * \brief  execpermfix test helper utility.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "execpermfix.h"

int main(int argc, char *argv[]) {
    const char *filename;
    mode_t pre;
    mode_t pos;
    struct stat st;

    (void)argc;

    filename = argv[1];
    pre = strtol(argv[2], NULL, 0);
    pos = strtol(argv[3], NULL, 0);

    chmod(filename, pre);
    execpermfix(filename, 0, 0);
    stat(filename, &st);
    printf("filename %s, pre 0%o, pos 0%o, curr 0%o\n", filename, pre, pos,
           0777 & st.st_mode);

    return pos == (0777 & st.st_mode) ? 0 : 1;
}

// Copyright (C) 2009 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

/**
 * \file
 * \brief  Fixes executable permissions of files and directories - header file.
 */

#ifndef EXECPERMFIX_H_INCLUDED
#define EXECPERMFIX_H_INCLUDED

#include <stdbool.h>
#include <sys/stat.h>

/**
 * \brief  Check if a file should BE executable by its contents.
 * \param  filename Path of file to process.
 * \param  fd File descriptor of entity (result of open).
 * \param  st Stat structure of entity (result of stat).
 * \return 0 if not executable, 1 if executable, -1 if error.
 */
int fileIsExec(const char *filename, int fd, const struct stat *st);

/**
 * \brief  Processes a file or a directory recursively.
 * \param  name Path of entity to process.
 * \param  noop Do not perform any change.
 * \param  verbose 1 to print actions.
 * \return 0 if ok, 1 if error.
 */
int execpermfix(const char *name, bool noop, bool verbose);

#endif /* EXECPERMFIX_H_INCLUDED */

// Copyright (C) 2009 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.

/**
 * \file
 * \brief  Fixes executable permissions of files and directories.
 */

#define _XOPEN_SOURCE 700  // swab
#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// #include "execpermfix.h"

/****************************************************************************/

static int filemapIsScript(const void *addr, size_t size) {
    const char scriptMagic[] = "#!";

    if (size >= (signed)(sizeof scriptMagic - 1) &&
        memcmp(addr, scriptMagic, sizeof scriptMagic - 1) == 0) {
        return 1;
    }

    return 0;
}

static int filemapIsElf(const char *filename, const void *addr, size_t size) {
    if (size < EI_NIDENT) return 0;

    if (memcmp(addr, ELFMAG, SELFMAG) != 0) {
        return 0;
    }

    const uint8_t *elfclass = (uint8_t *)addr + EI_CLASS;
    const uint8_t *elfdata = (uint8_t *)addr + EI_DATA;
    int e_type_norm = 0;

    if (*elfclass == ELFCLASS32) {
        const Elf32_Ehdr *hdr = addr;
        Elf32_Half e_type;
        assert(sizeof e_type == sizeof hdr->e_type);
        if (*elfdata == ELFDATA2LSB) {
            e_type = hdr->e_type;
        } else if (*elfdata == ELFDATA2MSB) {
            swab(&hdr->e_type, &e_type, sizeof e_type);
        } else {
            fprintf(stderr, "%s: unknown ELF data %d\n", filename, *elfdata);
            return -1;
        }
        e_type_norm = e_type;
    } else if (*elfclass == ELFCLASS64) {
        const Elf64_Ehdr *hdr = addr;
        Elf64_Half e_type;
        assert(sizeof e_type == sizeof hdr->e_type);
        if (*elfdata == ELFDATA2LSB) {
            e_type = hdr->e_type;
        } else if (*elfdata == ELFDATA2MSB) {
            swab(&hdr->e_type, &e_type, sizeof e_type);
        } else {
            fprintf(stderr, "%s: unknown ELF data %d\n", filename, *elfdata);
            return -1;
        }
        e_type_norm = e_type;
    } else {
        fprintf(stderr, "%s: unknown ELF class %d\n", filename, *elfclass);
        return -1;
    }

    if (e_type_norm == ET_EXEC || e_type_norm == ET_DYN) {
        return 1;
    } else if (e_type_norm == ET_REL || e_type_norm == ET_CORE ||
               e_type_norm == ET_LOPROC || e_type_norm == ET_HIPROC) {
        return 0;
    } else {
        fprintf(stderr, "%s: unknown ELF type %d\n", filename, e_type_norm);
        return -1;
    }

    return -1;
}

int fileIsExec(const char *filename, int fd, const struct stat *st) {
    size_t size = st->st_size;
    void *addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (addr == MAP_FAILED) {
        fprintf(stderr, "%s: mmap error %s\n", filename, strerror(errno));
        return -1;
    }

    int ret = filemapIsScript(addr, size);
    if (ret == 0) {
        ret = filemapIsElf(filename, addr, size);
    }

    if (munmap(addr, size) != 0) {
        fprintf(stderr, "%s: munmap error %s, keep going\n", filename,
                strerror(errno));
    }

    return ret;
}

/****************************************************************************/

int execpermfix(const char *name, int noop, bool verbose) {
    int fd = 0;
    struct stat st;
    char *fullname;
    int exec;
    mode_t mode;

    if ((fullname = realpath(name, NULL)) == NULL) {
        fprintf(stderr, "%s: realpath error %s\n", name, strerror(errno));
        return 1;
    }

    if ((fd = open(fullname, O_RDONLY)) < 0) {
        fprintf(stderr, "%s: open error %s\n", fullname, strerror(errno));
        free(fullname);
        return 1;
    }

    if (fstat(fd, &st) != 0) {
        fprintf(stderr, "%s: fstat error %s\n", fullname, strerror(errno));
        free(fullname);
        close(fd);
        return 1;
    }

    if (!S_ISDIR(st.st_mode) && !S_ISREG(st.st_mode)) {
        if (verbose)
            printf("%s skipped, neither a file nor a directory\n", fullname);
        free(fullname);
        close(fd);
        return 0;
    }

    /* Decide if file is executable: */
    exec = 0;
    if (S_ISDIR(st.st_mode))
        exec = 1;
    else if (S_ISREG(st.st_mode))
        exec = fileIsExec(fullname, fd, &st);
    if (exec < 0) {
        free(fullname);
        close(fd);
        return -1;
    }

    if (verbose) printf("%s: file is %sexecutable\n", name, exec ? "" : "not ");

    /* Define mode: */
    mode = st.st_mode;
    if (exec)
        mode |= (mode & 0444) >> 2;
    else
        mode &= ~0111;

    if (mode == st.st_mode) {
        if (verbose) printf("%s: %o would not be changed\n", name, st.st_mode);
        free(fullname);
        close(fd);
        return 0;
    }

    /* Set permission: */
    if (noop) {
        if (verbose)
            printf("%s: %o would change to %o\n", name, st.st_mode, mode);
        free(fullname);
        close(fd);
        return 0;
    }
    if (fchmod(fd, mode) < 0) {
        fprintf(stderr, "%s: chmod error %s\n", name, strerror(errno));
        free(fullname);
        close(fd);
        return 1;
    }
    if (verbose) printf("%s: %o changed to %o\n", name, st.st_mode, mode);

    free(fullname);
    close(fd);

    return 0;
}

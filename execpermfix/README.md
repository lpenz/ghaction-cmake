[![Build Status](https://travis-ci.org/lpenz/execpermfix.png?branch=master)](https://travis-ci.org/lpenz/execpermfix) 
[![Github workflow](https://github.com/lpenz/execpermfix/workflows/CI/badge.svg)](https://github.com/lpenz/execpermfix/actions)
[![codecov](https://codecov.io/gh/lpenz/execpermfix/branch/master/graph/badge.svg)](https://codecov.io/gh/lpenz/execpermfix)
[![packagecloud](https://img.shields.io/badge/deb-packagecloud.io-844fec.svg)](https://packagecloud.io/app/lpenz/lpenz/search?q=execpermfix)

execpermfix
===========

# About

execpermfix is a tool that fixes executable permissions of files and
directories.

It first finds out if the file is executable or not by checking if it starts
with the ELF magic number or "#!".

If the file is executable, execpermfix sets the executable bit of
user/group/other only if the read bit is set. If the file is not executable,
all executable bits are cleared.

Directories are always considered executable.


# Usage

~~~[.sh]
execpermfix -h
execpermfix -V
execpermfix [-v] [-n] <files/directories>
~~~

## Options

**-h** Help.

**-V** Version information.

**-v** Verbose, show files changed.

**-n** Dry-run: do not change any permissions. Use with ``-v`` to check file with wrong permissions.



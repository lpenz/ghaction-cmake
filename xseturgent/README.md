[![Build Status](https://travis-ci.org/lpenz/xseturgent.png?branch=master)](https://travis-ci.org/lpenz/xseturgent) 
[![codecov](https://codecov.io/gh/lpenz/xseturgent/branch/master/graph/badge.svg)](https://codecov.io/gh/lpenz/xseturgent)
[![packagecloud](https://img.shields.io/badge/deb-packagecloud.io-844fec.svg)](https://packagecloud.io/app/lpenz/lpenz/search?q=xseturgent)

xseturgent
==========

# About

xseturgent is a tool that sets/clears the urgency hint of a window in X11


# Usage

~~~[.sh]
xseturgent -h
xseturgent -V
xseturgent [-i <windowid>] [-t <set/reset/toggle>]
~~~

## Options

**-h** Help.

**-V** Version information.

**-v** Verbose, show files changed.

**-i windowid** The window ID, in decimal or hex. If not provided, use
   the WINDOWID environment variable by default; fails if that doesn't
   exist.

**-t <set/reset/toggle>** What to do with the urgency hint.


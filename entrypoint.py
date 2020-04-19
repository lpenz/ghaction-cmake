#!/usr/bin/env python3

import subprocess
import json
import os
print json.dumps(os.environ)

# if [ -n "$INPUT_DEPENDENCIES" ]; then
#     function install_dependencies {
#         su - -c "apt-get install --no-install-recommends -y $INPUT_DEPENDENCIES"
#     }
# else
#     function install_dependencies {
#         :
#     }
# fi

# CC="$INPUT_CC"
# CFLAGS="$INPUT_CFLAGS"
# CXXFLAGS="$INPUT_CXXFLAGS"
# if [ -n "${INPUT_COVERAGE}" ]; then
#     CFLAGS+=" --coverage"
#     CXXFLAGS+=" --coverage"
# fi

# ctest=( ctest )
# if [ -n "$INPUT_CTESTFLAGS" ]; then
#     ctest+=( "$INPUT_CTESTFLAGS" )
# fi

# set -e -x

# if [ -n "$INPUT_DIRECTORY" ]; then
#     cd "$INPUT_DIRECTORY"
# fi

# if [ -n "$INPUT_ANALYZER" ]; then
#     if [ "$INPUT_ANALYZER" = "clang-tidy" ]; then
#         install_dependencies
#         cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
#         find "$PWD" \( -name '*.[ch]' -o -name '*.cc' \) -exec clang-tidy -p "$PWD" {} +
#     elif [ "$INPUT_ANALYZER" = "clang-format" ]; then
#         find "$PWD" \( -name '*.[ch]' -o -name '*.cc' \) -exec clang-format -i {} +
#         git diff --exit-code
#     else
#         echo "Error: unknown analyzer $INPUT_ANALYZER" >&2
#         exit 1
#     fi
# else
#     install_dependencies
#     export CC="$CC"
#     export CFLAGS="$CFLAGS"
#     export CXXFLAGS="$CXXFLAGS"

#     SRCDIR="$PWD"
#     mkdir /tmp/_build /tmp/_install
#     cd /tmp/_build
#     cmake -DCMAKE_INSTALL_PREFIX=/tmp/_install "$SRCDIR"
#     make VERBOSE=1
#     "${ctest[@]}"
#     make install

#     if [ -n "${INPUT_COVERAGE}" ]; then
#         if [ "$INPUT_COVERAGE" = codecov ]; then
#             bash <(curl -s https://codecov.io/bash)
#         else
#             echo "Error: unknown coverage service $INPUT_COVERAGE" >&2
#             exit 1
#         fi
#     fi
# fi

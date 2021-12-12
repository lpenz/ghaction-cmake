[![marketplace](https://img.shields.io/badge/marketplace-cmake--swiss--army--knife-blue?logo=github)](https://github.com/marketplace/actions/cmake-swiss-army-knife)
[![CI](https://github.com/lpenz/ghaction-cmake/actions/workflows/ci.yml/badge.svg)](https://github.com/lpenz/ghaction-cmake/actions/workflows/ci.yml)
[![github](https://img.shields.io/github/v/release/lpenz/ghaction-cmake?include_prereleases&label=release&logo=github)](https://github.com/lpenz/ghaction-cmake/releases)
[![docker](https://img.shields.io/docker/v/lpenz/ghaction-cmake?label=release&logo=docker&sort=semver)](https://hub.docker.com/repository/docker/lpenz/ghaction-cmake)


# ghaction-cmake

**ghaction-cmake** is a github action for projects that
use [cmake](https://cmake.org/). By default, it builds, tests and
installs the project - but it can as easily run linters, tests with
coverage, valgrind or sanitizers, by using [presets](#preset).


## Phases

*ghaction-cmake* runs in phases:
- *setup*: optionally install dependencies and go to a specified
  directory.
- *cmake*: run cmake in an empty directory, pointing to the source
  directory, with all other arguments appended. This guarantees that
  out-of-tree builds work.
- *build*: customizable, `make VERBOSE=1` by default (build commands are shown).
- *test*: customizable, `ctest --output-on-failure .` by default.
- *post*: customizable, empty by default.


## Inputs

### `preset`

Set a preset, more information on the [Presets](#presets) section below.

- Phase: can changes the default command of any number of phases.

### `working-directory`

Use this directory as the source dir for cmake. Mostly used when the
cmake project is in a subdirectory of the repository.

- Phase: *setup*
- Preset behavior: unaffected.

### `pre_command`

Custom command to run before dependencies are installed. Use this to
add third-party Debian repositories or increment the setup phase in
other ways.

- Phase: *setup*
- Preset behavior: unaffected.

### `dependencies_debian`

Project dependencies as Debian packages to install in the container,
separated by spaces.

- Phase: *setup*
- Preset behavior: unaffected.

### `cmakeflags`

Flags for cmake. `-DSOME_OPTION=On`, for instance, to pass an option
to CMakeLists.txt.

- Phase: *cmake*
- Preset behavior: most presets append to this input.

### `build_command`

Custom test command. Defaults to `make VERBOSE=1`.

- Phase: *build*
- Preset behavior: some presets change or remove the default build
  command.

### `test_command`

Custom test command. Defaults to `ctest --output-on-failure .` if no preset is used.

- Phase: *test*
- Preset behavior: some presets change or remove the default test
  command.

### `post_command`

Custom command to run after tests. Empty by default, if no preset is used.

- Phase: *post*
- Preset behavior: some presets add a default post command.

## <a name="presets"></a> Presets

`cmake` is a very versatile tool that can do a lot of different things given
the appropriate arguments. To make matrix builds easier, *ghaction-cmake*
provides **presets** that configure those options for specific modes.

The available presets are:
- *cppcheck*: run [cppcheck](http://cppcheck.sourceforge.net/) static
  analysis.
  - *cmake*: append `-DCMAKE_C/CXX_CPPCHECK=cppcheck` to `cmakeflags`.
  - *test*: clear default.
- *iwyu*: run
  [include-what-you-use](https://include-what-you-use.org/) static
  analysis.
  - *cmake*: append `-DCMAKE_C/CXX_INCLUDE_WHAT_YOU_USE=iwyu` to `cmakeflags`.
  - *test*: clear default.
- *install*: test installation.
  - *cmake*: append `'-DCMAKE_INSTALL_PREFIX'` to `cmakeflags`.
  - *test*: use `make install` as a test.
  - *post*: use `find` to show all installed files.
- *clang-tidy*: run
  [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) static
  analysis.
  - *cmake*: append `-DCMAKE_C/CXX_CLANG_TIDY=clang-tidy` to `cmakeflags`.
  - *test*: clear default.
- *clang-sanitize-&lt;sanitizer&gt;*: compile with one of the
  [clang sanitizers](https://clang.llvm.org/docs/index.html) and
  run the tests.
  - *cmake*: append `-DCMAKE_C/CXX_COMPILER=clang/clang++ -DCMAKE_C/CXX_FLAGS=-fno-omit-frame-pointer -fsanitize=<sanitizer>` to `cmakeflags`.
- *valgrind*: run the tests with [valgrind](https://valgrind.org/).
  - *test*: set default test phase to `ctest -DExperimentalMemCheck --output-on-failure .`
- *cpack*: runs [cpack](https://cmake.org/cmake/help/latest/module/CPack.html)
  after the build.
  - *test*: cpack
- *coverage*: runs the tests with coverage.
  - *cmake*: append `-DCMAKE_C/CXX_FLAGS=--coverage` to `cmakeflags`
  - *post*: set default post phase to run
    [lcov](http://ltp.sourceforge.net/coverage/lcov.php) with
    `lcov -c -d . -o lcov.info`

  This preset works well with github actions that upload coverage data
  results to online services like
  [codecov](https://github.com/marketplace/actions/codecov) and
  [coveralls](https://github.com/marketplace/actions/coveralls-github-action).
  The [example](#example) below shows how that can be done.


The table below summarizes the changes specific to each preset:

<table>
<tr>
<th>Preset</th>
<th>cmake</th>
<th>test</th>
<th>post</th>
</tr>
<tr>
<td>cppcheck</td>
<td><pre>-DCMAKE_C/CXX_CPPCHECK=cppcheck</pre></td>
<td>(delete)</td>
<td></td>
</tr>
<tr>
<td>iwyu</td>
<td><pre>-DCMAKE_C/CXX_INCLUDE_WHAT_YOU_USE=iwyu</pre></td>
<td>(delete)</td>
<td></td>
</tr>
<tr>
<td>install</td>
<td><pre>-DCMAKE_INSTALL_PREFIX=/tmp/_install</pre></td>
<td><pre>make install</pre></td>
<td><pre>find /tmp_install -type f</pre></td>
</tr>
<tr>
<td>clang&#8209;tidy</td>
<td><pre>-DCMAKE_C/CXX_CLANG_TIDY=clang-tidy</pre></td>
<td>(delete)</td>
<td></td>
</tr>
<tr>
<td>clang&#8209;sanitizer&#8209;&lt;sanitizer&gt;</td>
<td>
<pre>-DCMAKE_C/CXX_COMPILER=clang/clang++
-DCMAKE_C/CXX_FLAGS=-fno-omit-frame-pointer -fsanitize=&lt;sanitizer&gt;</pre>
</td>
<td></td>
<td></td>
</tr>
<tr>
<td>valgrind</td>
<td></td>
<td><pre>-DExperimentalMemCheck</pre></td>
<td></td>
</tr>
<tr>
<td>cpack</td>
<td></td>
<td><pre>cpack</pre></td>
<td></td>
</tr>
<tr>
<td>coverage</td>
<td><pre>-DCMAKE_C/CXX_FLAGS=--coverage</pre></td>
<td></td>
<td><pre>lcov -c -d . -o lcov.info</pre></td>
</tr>
</table>

Keep in mind that presets override the defaults, and are overriden by
the other more specific inputs `build_command`, `test_command` and
`post_command`.


## <a name="example"></a> Example:

The workflow below shows how to use presets in a matrix job:

```yml
---
name: CI
on: [push, pull_request]
jobs:
  # Regular C build with two compilers, using the environment:
  build_using_compiler_in_environment:
    strategy:
        matrix:
          compiler:
            - gcc
            - clang
    runs-on: ubuntu-latest
    # We can use cmakeflags for this, or we can just use
    # regular environment variables, as they are already
    # supported by github actions:
    env:
      - CC: ${{ matrix.compiler }}
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
  # Regular C build with two compilers, using cmakeflags:
  build_using_compiler_in_cmakeflags:
    strategy:
        matrix:
          compiler:
            - gcc
            - clang
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      # This examples uses the appropriate cmakeflags
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          cmakeflags: ${{ format('-DCMAKE_C_COMPILER={0}', matrix.compiler) }}
  # Coverage with codecov:
  codecov:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          preset: coverage
      # ghaction-cmake works well with the github action
      # provided by codecov:
      - uses: codecov/codecov-action@v1
        with:
          fail_ci_if_error: true
  # Coverage with coveralls:
  coveralls:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          preset: coverage
      # ghaction-cmake works well with the github action
      # provided by coveralls if you pass path-to-lcov:
      - uses: coverallsapp/github-action@master
        with:
          github-token: ${{ secrets.GITHUB_TOKEN }}
          path-to-lcov: lcov.info
  # Static analyzers:
  linters:
    strategy:
        matrix:
          preset: [ cppcheck, iwyu, clang-tidy ]
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          preset: ${{ matrix.preset }}
  # Tests with various sanitizers and valgrind:
  test:
    strategy:
        matrix:
          preset:
            - clang-sanitizer-address
            - clang-sanitizer-memory
            - clang-sanitizer-undefined
            - clang-sanitizer-dataflow
            - clang-sanitizer-safe-stack
            - valgrind
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          preset: ${{ matrix.preset }}
  # Test installation:
  install:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: docker://lpenz/ghaction-cmake:0.16
        with:
          preset: install
```

Note that the file above splits static analyzers from sanitizers, but
they can actually be in the same matrix job, as the rest of the
parameters is the same.


## Using in other environments

This github action is actually a docker image that can be used locally
or even in [travis-ci](https://travis-ci.com). To do that, first
download the image from
[docker hub](https://hub.docker.com/r/lpenz/ghaction-cmake):

```sh
docker pull lpenz/ghaction-cmake:0.16
```

Then, run a container in the project's directory, for instance:

```sh
docker run --rm -t -u "$UID" -w "$PWD" -v "${PWD}:${PWD}" -e INPUT_PRESET=valgrind lpenz/ghaction-cmake:0.16
```

It's worth pointing out that action parameters are passed as
upper case environment variables, prefixed with `INPUT_`.

The following `.travis.yml` runs the same thing in travis-ci:

```yml
---
language: generic
jobs:
  include:
    - install: docker pull lpenz/ghaction-cmake:0.16
    - script: docker run --rm -t -u "$UID" -w "$PWD" -v "${PWD}:${PWD}" -e INPUT_PRESET=valgrind lpenz/ghaction-cmake:0.16
```

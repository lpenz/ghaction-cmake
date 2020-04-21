#!/usr/bin/env python3 -u

import subprocess
import json
import os, sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


if os.environ["INPUT_VERBOSE"] == "true":
    verbose = True
    eprint("Environment: ", json.dumps(os.environ.copy(), indent=4, sort_keys=True))
else:
    verbose = False


def check_call(*args, **kwargs):
    if verbose:
        eprint("Calling subprocess: ", args)
    subprocess.check_call(*args, **kwargs)


extra_dependencies = os.environ.get("INPUT_DEPENDENCIES", "").split()
if len(extra_dependencies) > 0:
    args = ["apt-get", "install", "--no-install-recommends", "-y"]
    args.extend(extra_dependencies)
    check_call(args)

callenv = os.environ.copy()
callenv["CFLAGS"] = callenv.get("CFLAGS", "")
callenv["CXXFLAGS"] = callenv.get("CXXFLAGS", "")

ctest_coverage = os.environ["INPUT_CTEST_COVERAGE"] == "true"

if ctest_coverage:
    callenv["CFLAGS"] += " --coverage"
    callenv["CXXFLAGS"] += " --coverage"

source_directory = os.environ["INPUT_SOURCE_DIRECTORY"]
build_directory = os.environ["INPUT_BUILD_DIRECTORY"]

if verbose:
    eprint(
        "Creating directories: src:",
        source_directory,
        ", build: ",
        build_directory,
        ", in cwd=",
        os.getcwd(),
    )

os.makedirs(os.path.join(os.getcwd(), source_directory), exist_ok=True)
os.makedirs(os.path.join(os.getcwd(), build_directory), exist_ok=True)

cmake_configure_commands = ["cmake", "-S", source_directory, "-B", build_directory]
cmake_configure_commands.append("-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")

cmake_configure_sanitize = os.environ["INPUT_CMAKE_CONFIGURE_SANITIZE"]
if len(cmake_configure_sanitize) > 1:
    callenv["CFLAGS"] += " -fsanitize=" + cmake_configure_sanitize
    callenv["CXXFLAGS"] += " -fsanitize=" + cmake_configure_sanitize

cmake_configure_environment = os.environ["INPUT_CMAKE_CONFIGURE_ENVIRONMENT"]
if verbose:
    eprint("INPUT_CMAKE_CONFIGURE_ENVIRONMENT=", cmake_configure_environment)

for cenv in cmake_configure_environment.split():
    if verbose:
        eprint("Adding entry to call environment:", cenv)

    var, val = cenv.split("=", 1)

    if verbose:
        eprint("callenv[", var, "] = ", val)
    callenv[var] = val

cmake_configure_extra_commands = os.environ["INPUT_CMAKE_CONFIGURE_EXTRA_ARGS"].split()
cmake_configure_commands.extend(cmake_configure_extra_commands)

if verbose:
    eprint("CMake configure env: ", json.dumps(callenv, indent=4, sort_keys=True))
check_call(cmake_configure_commands, env=callenv)

if os.environ["INPUT_CMAKE_BUILD_ENABLE"] == "true":
    cmake_build_options = os.environ.get("INPUT_CMAKE_BUILD_OPTIONS", "").split()
    args = ["cmake", "--build", build_directory]
    args.extend(cmake_build_options)
    check_call(args)

if os.environ["INPUT_CTEST_ENABLE"] == "true":
    args = ["ctest"]
    args.extend(os.environ.get("INPUT_CTEST_FLAGS", "").split())
    check_call(args, cwd=build_directory)

if os.environ["INPUT_CMAKE_INSTALL_ENABLE"] == "true":
    args = [
        "cmake",
        "--install",
        build_directory,
        "--prefix",
        os.environ["INPUT_CMAKE_INSTALL_DIRECTORY"],
    ]
    cmake_install_options = os.environ.get("INPUT_CMAKE_INSTALL_OPTIONS", "")
    args.extend(cmake_install_options)
    check_call(args)

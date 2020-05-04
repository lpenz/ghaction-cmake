# Copyright (C) 2014 Leandro Lisboa Penz <lpenz@lpenz.org>
# This file is subject to the terms and conditions defined in
# file 'LICENSE.txt', which is part of this source code package.

cmake_minimum_required (VERSION 2.6)

project (execpermfix)

include(GNUInstallDirs)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Main executable::
include_directories(include)
add_executable(execpermfix src/execpermfix.c src/execpermfix_main.c)

# Manual:
add_custom_command(
	OUTPUT execpermfix.1.gz
	DEPENDS execpermfix.1
	COMMAND gzip -9kc
		${CMAKE_CURRENT_SOURCE_DIR}/execpermfix.1 >
		${CMAKE_CURRENT_BINARY_DIR}/execpermfix.1.gz
	)
add_custom_target(manual ALL DEPENDS execpermfix.1.gz)

# Install:
install (
    TARGETS execpermfix
    DESTINATION bin
    )
install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/execpermfix.1.gz
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/
    )

# Tests:
include(CTest)

add_executable(testhelper test/testhelper.c src/execpermfix.c)

add_test (NAME test_execpermfix_help
	COMMAND execpermfix -h)
set_tests_properties(test_execpermfix_help PROPERTIES PASS_REGULAR_EXPRESSION "Usage:")

add_test (NAME test_execpermfix_help2
	COMMAND execpermfix -?)
set_tests_properties(test_execpermfix_help2 PROPERTIES WILL_FAIL true)

add_test (NAME test_execpermfix_version
	COMMAND execpermfix -V)

add_test (NAME test_execpermfix_invalid
	COMMAND execpermfix -?)
set_tests_properties(test_execpermfix_invalid PROPERTIES WILL_FAIL true)

add_test (NAME test_execpermfix_null
	COMMAND execpermfix /dev/null)

add_test (NAME test_execpermfix_notfound
	COMMAND execpermfix /notfound)
set_tests_properties(test_execpermfix_notfound PROPERTIES WILL_FAIL true)

execute_process(COMMAND mkdir -p _tmp/dir)
execute_process(COMMAND printf \#!/bin/sh\n\n OUTPUT_FILE _tmp/script)
execute_process(COMMAND printf bin/sh\n\n OUTPUT_FILE _tmp/none)
execute_process(COMMAND cp -L /bin/ls _tmp OUTPUT_FILE _tmp/ls)

add_test (NAME test_execpermfix_script_setU    COMMAND ./testhelper _tmp/script 0600 0700)
add_test (NAME test_execpermfix_script_setG    COMMAND ./testhelper _tmp/script 0640 0750)
add_test (NAME test_execpermfix_script_setO    COMMAND ./testhelper _tmp/script 0644 0755)
add_test (NAME test_execpermfix_script_keeU    COMMAND ./testhelper _tmp/script 0700 0700)
add_test (NAME test_execpermfix_script_keeG    COMMAND ./testhelper _tmp/script 0750 0750)
add_test (NAME test_execpermfix_script_keeO    COMMAND ./testhelper _tmp/script 0755 0755)
add_test (NAME test_execpermfix_binary_setU    COMMAND ./testhelper _tmp/ls 0600 0700)
add_test (NAME test_execpermfix_binary_setG    COMMAND ./testhelper _tmp/ls 0640 0750)
add_test (NAME test_execpermfix_binary_setO    COMMAND ./testhelper _tmp/ls 0644 0755)
add_test (NAME test_execpermfix_binary_keeU    COMMAND ./testhelper _tmp/ls 0700 0700)
add_test (NAME test_execpermfix_binary_keeG    COMMAND ./testhelper _tmp/ls 0750 0750)
add_test (NAME test_execpermfix_binary_keeO    COMMAND ./testhelper _tmp/ls 0755 0755)
add_test (NAME test_execpermfix_directory_setU COMMAND ./testhelper _tmp/dir 0600 0700)
add_test (NAME test_execpermfix_directory_setG COMMAND ./testhelper _tmp/dir 0640 0750)
add_test (NAME test_execpermfix_directory_setO COMMAND ./testhelper _tmp/dir 0644 0755)
add_test (NAME test_execpermfix_directory_keeU COMMAND ./testhelper _tmp/dir 0700 0700)
add_test (NAME test_execpermfix_directory_keeG COMMAND ./testhelper _tmp/dir 0750 0750)
add_test (NAME test_execpermfix_directory_keeO COMMAND ./testhelper _tmp/dir 0755 0755)
add_test (NAME test_execpermfix_none_clearU    COMMAND ./testhelper _tmp/none 0700 0600)
add_test (NAME test_execpermfix_none_clearG    COMMAND ./testhelper _tmp/none 0750 0640)
add_test (NAME test_execpermfix_none_clearO    COMMAND ./testhelper _tmp/none 0755 0644)
add_test (NAME test_execpermfix_none_keepU     COMMAND ./testhelper _tmp/none 0600 0600)
add_test (NAME test_execpermfix_none_keepG     COMMAND ./testhelper _tmp/none 0640 0640)
add_test (NAME test_execpermfix_none_keepO     COMMAND ./testhelper _tmp/none 0644 0644)


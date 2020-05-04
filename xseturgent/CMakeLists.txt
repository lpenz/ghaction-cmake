
cmake_minimum_required (VERSION 3.0)

project (xseturgent)

include(GNUInstallDirs)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package (PkgConfig REQUIRED)
pkg_check_modules (X11 REQUIRED x11)
link_libraries ("${X11_LIBRARIES}")

# Main executable:
add_executable (xseturgent xseturgent.c)

# Manual:
add_custom_command(
	OUTPUT xseturgent.1.gz
	DEPENDS xseturgent.1
	COMMAND gzip -9kc
		${CMAKE_CURRENT_SOURCE_DIR}/xseturgent.1 >
		${CMAKE_CURRENT_BINARY_DIR}/xseturgent.1.gz
	)
add_custom_target(manual ALL DEPENDS xseturgent.1.gz)

# Install:
install (TARGETS xseturgent
  RUNTIME DESTINATION bin/
  )
install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/xseturgent.1.gz
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/
  )

# Tests:
include(CTest)

add_test (NAME bare    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh)
add_test (NAME verbose COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh -v)
add_test (NAME dashi   COMMAND /bin/bash -c "${CMAKE_CURRENT_SOURCE_DIR}/test.sh -v -i '$WINDOWID'")
add_test (NAME toggle  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh -v -t toggle)
add_test (NAME set     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh -v -t set)
add_test (NAME reset   COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh -v -t reset)

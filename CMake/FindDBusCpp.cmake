# 

# - Try to find DBusCpp
# Once done this will define
#  DBUSCPP_FOUND - System has DBusCpp
#  DBUSCPP_INCLUDE_DIRS - The DBusCpp include directories
#  DBUSCPP_LIBRARIES - The libraries needed to use DBusCpp
#  DBUSCPP_DEFINITIONS - Compiler switches required for using DBusCpp

#find_package(PkgConfig)
#pkg_check_modules(PC_DBUSCPP QUIET dbus-c++-1.pc)
#set(DBUSCPP_DEFINITIONS ${PC_DBUSCPP_CFLAGS_OTHER})

find_path(DBUSCPP_INCLUDE_DIR dbus-c++-1/dbus-c++/dbus.h
          HINTS ${PC_DBUSCPP_INCLUDEDIR} ${PC_DBUSCPP_INCLUDE_DIRS}
          PATH_SUFFIXES dbuscpp )

find_library(DBUSCPP_LIBRARY NAMES libdbus-c++-1.so
             HINTS ${PC_DBUSCPP_LIBDIR} ${PC_DBUSCPP_LIBRARY_DIRS} )

set(DBUSCPP_LIBRARIES ${DBUSCPP_LIBRARY} )
set(DBUSCPP_INCLUDE_DIRS ${DBUSCPP_INCLUDE_DIR} )

# DBUSCPP_INCLUDE_DIRS is /usr/include.
include_directories(${DBUSCPP_INCLUDE_DIRS}/dbus-c++-1)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set DBUSCPP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(DBusCpp  DEFAULT_MSG
                                  DBUSCPP_LIBRARY DBUSCPP_INCLUDE_DIR)
mark_as_advanced(DBUSCPP_INCLUDE_DIR DBUSCPP_LIBRARY )

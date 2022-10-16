# 

# - Try to find Gloox
# Once done this will define
#  GLOOX_FOUND - System has Gloox
#  GLOOX_INCLUDE_DIRS - The Gloox include directories
#  GLOOX_LIBRARIES - The libraries needed to use Gloox
#  GLOOX_DEFINITIONS - Compiler switches required for using Gloox

find_package(PkgConfig)
pkg_check_modules(PC_GLOOX QUIET gloox.pc)
set(GLOOX_DEFINITIONS ${PC_GLOOX_CFLAGS_OTHER})

find_path(GLOOX_INCLUDE_DIR gloox/gloox.h
          HINTS ${PC_GLOOX_INCLUDEDIR} ${PC_GLOOX_INCLUDE_DIRS}
          PATH_SUFFIXES gloox )

find_library(GLOOX_LIBRARY NAMES libgloox.so
             HINTS ${PC_GLOOX_LIBDIR} ${PC_GLOOX_LIBRARY_DIRS} )

set(GLOOX_LIBRARIES ${GLOOX_LIBRARY} )
set(GLOOX_INCLUDE_DIRS ${GLOOX_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GLOOX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Gloox  DEFAULT_MSG
                                  GLOOX_LIBRARY GLOOX_INCLUDE_DIR)
mark_as_advanced(GLOOX_INCLUDE_DIR GLOOX_LIBRARY )

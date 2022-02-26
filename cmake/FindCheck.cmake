# This file is licensed under the WTFPL version 2 -- you can see the full
# license over at http://www.wtfpl.net/txt/copying/
#
# - Try to find Check
#
# Once done this will define
#  CHECK_FOUND - System has Check
#  CHECK_INCLUDE_DIRS - The Check include directories
#  CHECK_LIBRARIES - The libraries needed to use Check

find_package(PkgConfig)

find_path(CHECK_INCLUDE_DIR check.h
          PATH_SUFFIXES check)

find_library(CHECK_LIBRARY NAMES check libcriterion)

set(CHECK_LIBRARIES ${CHECK_LIBRARY})
set(CHECK_INCLUDE_DIRS ${CHECK_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set CHECK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Check DEFAULT_MSG
                                  CHECK_LIBRARY CHECK_INCLUDE_DIR)

mark_as_advanced(CHECK_INCLUDE_DIR CHECK_LIBRARY)

# -*- cmake -*-

# - Find JSONCpp
# Find the JSONCpp includes and library
# This module defines
# JSONCPP_INCLUDE_DIR, where to find json.h, etc.
# JSONCPP_LIBRARIES, the libraries needed to use jsoncpp.
# JSONCPP_FOUND, If false, do not try to use jsoncpp.
# also defined, but not for general use are
# JSONCPP_LIBRARY, where to find the jsoncpp library.
#

FIND_PATH(JSONCPP_INCLUDE_DIR json/json.h
  /usr/local/include/jsoncpp
  /usr/include/jsoncpp
  /afs/csail.mit.edu/u/x/xuyan/lib/json-cpp/jsoncpp-src-0.5.0
  /afs/csail.mit.edu/u/x/xuyan/lib/json-cpp/jsoncpp-src-0.5.0/include
  /afs/csail.mit.edu/u/x/xuyan/lib/json-cpp/jsoncpp-src-0.5.0/include/json
)

# Get the GCC compiler version
EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
OUTPUT_VARIABLE _gcc_COMPILER_VERSION
OUTPUT_STRIP_TRAILING_WHITESPACE
)

SET(JSONCPP_NAMES ${JSONCPP_NAMES} libjsoncpp.a libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.a)
FIND_LIBRARY(JSONCPP_LIBRARY
NAMES ${JSONCPP_NAMES}
PATHS /usr/lib /usr/local/lib /afs/csail.mit.edu/u/x/xuyan/lib/json-cpp/jsoncpp-src-0.5.0/libs/linux-gcc-4.4.5
)

IF (JSONCPP_LIBRARY AND JSONCPP_INCLUDE_DIR)
SET(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
SET(JSONCPP_FOUND "YES")
ELSE (JSONCPP_LIBRARY AND JSONCPP_INCLUDE_DIR)
SET(JSONCPP_FOUND "NO")
ENDIF (JSONCPP_LIBRARY AND JSONCPP_INCLUDE_DIR)


IF (JSONCPP_FOUND)
IF (NOT JSONCPP_FIND_QUIETLY)
MESSAGE(STATUS "Found JSONCpp: ${JSONCPP_LIBRARIES} and ${JSONCPP_INCLUDE_DIR}")
ENDIF (NOT JSONCPP_FIND_QUIETLY)
ELSE (JSONCPP_FOUND)
IF (JSONCPP_FIND_REQUIRED)
MESSAGE(FATAL_ERROR "Could not find JSONCpp library")
ENDIF (JSONCPP_FIND_REQUIRED)
ENDIF (JSONCPP_FOUND)

# Deprecated declarations.
SET (NATIVE_JSONCPP_INCLUDE_PATH ${JSONCPP_INCLUDE_DIR} )
GET_FILENAME_COMPONENT (NATIVE_JSONCPP_LIB_PATH ${JSONCPP_LIBRARY} PATH)

MARK_AS_ADVANCED(
JSONCPP_LIBRARY
JSONCPP_INCLUDE_DIR
)

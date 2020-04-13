# Locate the glad library
#
# This module defines the following variables:
#
# GLAD_LIBRARY the name of the library;
# GLAD_INCLUDE_DIR where to find glad include files.
# GLAD_FOUND true if both the GLAD_LIBRARY and GLAD_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you can define a
# variable called GLAD_ROOT which points to the root of the glad library
# installation.
#
# default search dirs
# 

set( _glad_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes"
"C:/Program Files (x86)/glad/include" )
set( _glad_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
"C:/Program Files (x86)/glad/lib-msvc110" )

# Check environment for root search directory
set( _glad_ENV_ROOT $ENV{GLAD_ROOT} )
if( NOT GLAD_ROOT AND _glad_ENV_ROOT )
	set(GLAD_ROOT ${_glad_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( GLAD_ROOT )
	list( INSERT _glad_HEADER_SEARCH_DIRS 0 "${GLAD_ROOT}/include" )
	list( INSERT _glad_LIB_SEARCH_DIRS 0 "${GLAD_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(GLAD_INCLUDE_DIR "glad/glad.h"
PATHS ${_glad_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(GLAD_LIBRARY NAMES glad glad
PATHS ${_glad_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLAD DEFAULT_MSG
GLAD_LIBRARY GLAD_INCLUDE_DIR)
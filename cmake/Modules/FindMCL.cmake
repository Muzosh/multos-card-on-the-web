# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindMCL
--------

Find the MCL cryptography library.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines the following :prop_tgt:`IMPORTED` targets:

``MCL::MCL``
  The MCL ``mcl`` library, if found.
``MCL::Bn256``
  The MCL ``mclbn256`` library, if found.
``MCL::Bn384_256``
  The MCL ``mclbn384_256`` library, if found.
``MCL::Bn384``
  The MCL ``mclbn384`` library, if found.

Result Variables
^^^^^^^^^^^^^^^^

This module will set the following variables in your project:

``MCL_FOUND``
  System has the MCL library.
``MCL_INCLUDE_DIR``
  The MCL include directory.
``MCL_LIBRARY``
  The MCL library.
``MCL_BN256_LIBRARY``
  The MCL Bn256 library.
``MCL_BN384_256_LIBRARY``
  The MCL Bn384_256 library.
``MCL_BN384_LIBRARY``
  The MCL Bn384 library.
``MCL_VERSION``
  the MCL library version.

Hints
^^^^^

Set ``MCL_ROOT_DIR`` to the root directory of an MCL installation.
#]=======================================================================]

find_path(MCL_INCLUDE_DIR
  NAMES
    mcl/bn.h
    mcl/bn_c256.h
    mcl/bn_c384_256.h
    mcl/bn_c384.h
  PATHS
    ${MCL_ROOT_DIR}
  PATH_SUFFIXES
    include
)

find_library(MCL_LIBRARY
  NAMES
    mcl
  PATHS
    ${MCL_ROOT_DIR}
  PATH_SUFFIXES
    lib
)

find_library(MCL_BN256_LIBRARY
  NAMES
    mclbn256
  PATHS
    ${MCL_ROOT_DIR}
  PATH_SUFFIXES
    lib
)

find_library(MCL_BN384_256_LIBRARY
  NAMES
    mclbn384_256
  PATHS
    ${MCL_ROOT_DIR}
  PATH_SUFFIXES
    lib
)

find_library(MCL_BN384_LIBRARY
  NAMES
    mclbn384
  PATHS
    ${MCL_ROOT_DIR}
  PATH_SUFFIXES
    lib
)

if (MCL_INCLUDE_DIR AND EXISTS "${MCL_INCLUDE_DIR}/mcl/op.hpp")
  file(STRINGS "${MCL_INCLUDE_DIR}/mcl/op.hpp" mcl_version_str
    REGEX "^[\t ]*static const int version = 0x[0-9]+")
  string(REGEX REPLACE "^.*version = 0x([0-9])([0-9][0-9]).*$"
    "\\1;\\2" MCL_VERSION_LIST "${mcl_version_str}")
  list(GET MCL_VERSION_LIST 0 MCL_VERSION_MAJOR)
  list(GET MCL_VERSION_LIST 1 MCL_VERSION_MINOR)
  set(MCL_VERSION "${MCL_VERSION_MAJOR}.${MCL_VERSION_MINOR}")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MCL
  REQUIRED_VARS
    MCL_LIBRARY
    MCL_BN256_LIBRARY
    MCL_BN384_256_LIBRARY
    MCL_BN384_LIBRARY
    MCL_INCLUDE_DIR
  VERSION_VAR
    MCL_VERSION
  HANDLE_COMPONENTS
  FAIL_MESSAGE
    "Could NOT find MCL, try to set the path to MCL root folder in the system variable MCL_ROOT_DIR"
)

if (MCL_FOUND)
  if (NOT TARGET MCL::MCL AND (EXISTS "${MCL_LIBRARY}"))
    add_library(MCL::MCL UNKNOWN IMPORTED)
    set_target_properties(MCL::MCL PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${MCL_INCLUDE_DIR}")
    if (EXISTS "${MCL_LIBRARY}")
      set_target_properties(MCL::MCL PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${MCL_LIBRARY}")
    endif ()
  endif ()
  if (NOT TARGET MCL::Bn256 AND (EXISTS "${MCL_BN256_LIBRARY}"))
    add_library(MCL::Bn256 UNKNOWN IMPORTED)
    set_target_properties(MCL::Bn256 PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${MCL_INCLUDE_DIR}")
    if (EXISTS "${MCL_BN256_LIBRARY}")
      set_target_properties(MCL::Bn256 PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${MCL_BN256_LIBRARY}")
    endif ()
  endif ()
  if (NOT TARGET MCL::Bn384_256 AND (EXISTS "${MCL_BN384_256_LIBRARY}"))
    add_library(MCL::Bn384_256 UNKNOWN IMPORTED)
    set_target_properties(MCL::Bn384_256 PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${MCL_INCLUDE_DIR}")
    if (EXISTS "${MCL_BN384_256_LIBRARY}")
      set_target_properties(MCL::Bn384_256 PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${MCL_BN384_256_LIBRARY}")
    endif ()
  endif ()
  if (NOT TARGET MCL::Bn384 AND (EXISTS "${MCL_BN384_LIBRARY}"))
    add_library(MCL::Bn384 UNKNOWN IMPORTED)
    set_target_properties(MCL::Bn384 PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${MCL_INCLUDE_DIR}")
    if (EXISTS "${MCL_BN384_LIBRARY}")
      set_target_properties(MCL::Bn384 PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${MCL_BN384_LIBRARY}")
    endif ()
  endif ()
endif ()

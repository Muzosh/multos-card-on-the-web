# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindPCSC
--------

Find the PCSC middleware library to access a smart card using SCard API (PC/SC).

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines the following :prop_tgt:`IMPORTED` targets:

``PCSC::PCSC``
  The PCSC ``pcsc`` library, if found.

Result Variables
^^^^^^^^^^^^^^^^

This module will set the following variables in your project:

``PCSC_FOUND``
  System has the PCSC library.
``PCSC_INCLUDE_DIR``
  The PCSC include directory.
``PCSC_LIBRARY``
  The PCSC library.
``PCSC_VERSION``
  the PCSC library version.

Hints
^^^^^

Set ``PCSC_ROOT_DIR`` to the root directory of an PCSC installation.
#]=======================================================================]

find_path(PCSC_INCLUDE_DIR
  NAMES
    PCSC/pcsclite.h
    PCSC/winscard.h
    PCSC/wintypes.h
  PATHS
    ${PCSC_ROOT_DIR}
  PATH_SUFFIXES
    include
)

find_library(PCSC_LIBRARY
  NAMES
    pcsclite
    libpcsclite
    PCSC
  PATHS
    ${PCSC_ROOT_DIR}
  PATH_SUFFIXES
    lib
)

if (PCSC_INCLUDE_DIR)
  if (PCSC_LIBRARY MATCHES "/([^/]+)\\.framework$" AND EXISTS "${PCSC_INCLUDE_DIR}/Headers/pcsclite.h")
    file(STRINGS "${PCSC_INCLUDE_DIR}/Headers/pcsclite.h" pcsc_version_str
      REGEX "^#[\t ]*define[\t ]+PCSCLITE_VERSION_NUMBER[\t ]+\"([0-9])+.([0-9])+.([0-9])+\"")
  elseif (EXISTS "${PCSC_INCLUDE_DIR}/PCSC/pcsclite.h")
    file(STRINGS "${PCSC_INCLUDE_DIR}/PCSC/pcsclite.h" pcsc_version_str
      REGEX "^#[\t ]*define[\t ]+PCSCLITE_VERSION_NUMBER[\t ]+\"([0-9])+.([0-9])+.([0-9])+\"")
  endif ()
  string(REGEX REPLACE "^.*PCSCLITE_VERSION_NUMBER[\t ]+\"([0-9]+.[0-9]+.[0-9]+)\".*$"
    "\\1" PCSC_VERSION "${pcsc_version_str}")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCSC
  REQUIRED_VARS
   PCSC_LIBRARY
   PCSC_INCLUDE_DIR
  VERSION_VAR
   PCSC_VERSION
  HANDLE_COMPONENTS
  FAIL_MESSAGE
   "Could NOT find PCSC, try to set the path to PCSC root folder in the system variable PCSC_ROOT_DIR"
)

if (PCSC_FOUND)
  if (NOT TARGET PCSC::PCSC AND (EXISTS "${PCSC_LIBRARY}"))
    if (PCSC_LIBRARY MATCHES "/([^/]+)\\.framework$")
      add_library(PCSC::PCSC INTERFACE IMPORTED)
      set_target_properties(PCSC::PCSC PROPERTIES
        INTERFACE_LINK_LIBRARIES "${PCSC_LIBRARY}")
    else ()
      add_library(PCSC::PCSC UNKNOWN IMPORTED)
      set_target_properties(PCSC::PCSC PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${PCSC_LIBRARY}")
    endif ()
    set_target_properties(PCSC::PCSC PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${PCSC_INCLUDE_DIR}")
  endif ()
endif ()

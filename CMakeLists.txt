cmake_minimum_required(VERSION 3.17)
project(hello-world C)

set(CMAKE_C_STANDARD 90)

# SmartCard support options
option(PROTOCOL_MULTOS "MultOS version" OFF)


# Custom CMake Modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenSSL REQUIRED) # OpenSSL package
find_package(MCL REQUIRED) # MCL package

if (PROTOCOL_MULTOS)
  find_package(PCSC REQUIRED) # required to communicate with SmartCards
endif ()


# Project-level includes
include_directories(. ./include ./lib ./src)

set(EXECUTABLE_COMMON_SOURCE
  config/config.h
  include/models/user.h
  include/system.h
  include/types.h
  lib/helpers/hex_helper.c
  lib/helpers/hex_helper.h
  lib/helpers/mcl_helper.c
  lib/helpers/mcl_helper.h
  src/setup.c
  src/setup.h
  main.c
)


# PC binary
add_executable(hello-world ${EXECUTABLE_COMMON_SOURCE}
  src/controllers/user.c
  src/controllers/user.h
)
target_link_libraries(hello-world PRIVATE MCL::Bn256 OpenSSL::Crypto)
target_compile_definitions(hello-world PRIVATE)


# MULTOS binary
if (PROTOCOL_MULTOS)
  add_executable(hello-world-multos ${EXECUTABLE_COMMON_SOURCE}
          include/multos/apdu.h
    lib/apdu/command.c
    lib/apdu/command.h
    lib/helpers/multos_helper.c
    lib/helpers/multos_helper.h
    lib/pcsc/reader.c
    lib/pcsc/reader.h
    src/controllers/multos/user.c
    src/controllers/multos/user.h
  )
  target_link_libraries(hello-world-multos PRIVATE MCL::Bn256 OpenSSL::Crypto PCSC::PCSC)
  target_compile_definitions(hello-world-multos PRIVATE PROTOCOL_MULTOS)
endif ()

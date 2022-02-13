# Preamble
cmake_minimum_required(VERSION 3.20)

# VCPKG
option(USE_VCPKG "Fetch dependencies using the vcpkg package manager" OFF)
if(USE_VCPKG)
  set(CMAKE_TOOLCHAIN_FILE
      ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
      CACHE STRING "Vcpkg toolchain file")
endif()

project(wyag)
enable_testing()

# Project wide setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_GENERATOR Ninja)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror") # enforce stringent
                                                        # build requirements
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # move runtime
                                                            # targets into one
                                                            # folder
include(AssureOutOfSourceBuilds) # enforce out of source builds

# Externally provided content
add_subdirectory(deps)
include(packages)

# Main targets built by this project
add_subdirectory(src)

# Testing and Installing
option(BUILD_TESTS "Build unit tests." ON)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()

# Documentation with doxygen
option(BUILD_DOC "Build documentation." OFF)
if(BUILD_DOC)
  add_subdirectory(docs)
endif()

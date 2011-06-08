
cmake_minimum_required(VERSION 2.8)

project(routeme C)

include(FindPkgConfig)

pkg_check_modules(GLIB REQUIRED "glib-2.0 >= 2.28.6")
pkg_check_modules(GOBJECT REQUIRED "gobject-2.0 >= 2.28.6")

find_program(GOB gob2)
if (NOT GOB)
    message(FATAL_ERROR "GObjectBuilder is a required dependency")
endif()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

option(TESTS "Compile and run unit tests" OFF)
if (TESTS)
    #    pkg_check_modules(CHECK REQUIRED "check >= 0.9.8")
    find_library(CUNIT cunit)
    if (CUNIT)
        set(CUNIT_LIBRARIES cunit)
    else()
        message(FATAL_ERROR "CUnit is a required dependency")
    endif()
endif()

include(CheckFunctionExists)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

configure_file(config.h.in config.h)

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)


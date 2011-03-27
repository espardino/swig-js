cmake_minimum_required(VERSION 2.8)

project(swig-js)

# Options:

# set(TARGET_ARCHITECTURE "ia32" CACHE STRING "arm, ia32, x64, mips")

# option(V8_SNAPSHOT "v8: use snapshots" ON)
# option(V8_OPROFILE "v8: use oprofile" ON)
# option(V8_GDBJIT "v8: use gdbjit" ON)

#
# Boost:
# -------
#
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/FireBreath/src/3rdParty/boost" CACHE PATH "Path to boost")

#
# SVN:
# ----
#

find_package(Subversion)
if(NOT ${Subversion_FOUND})
    message(FATAL_ERROR "Could not find subversion (required).")
endif()

#
# Scons:
# ------
#

find_program(Scons_EXECUTABLE scons)
if(${Scons_EXECUTABLE} EQUAL "Scons_EXECUTABLE-NOTFOUND")
    message(FATAL_ERROR "Could not find scons (required).")
else()
    message("Found scons: ${Scons_EXECUTABLE}")
endif()

#
# Internal variables:
# ===================
#

set(FIREBREATH_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/FireBreath" CACHE FILEPATH "Path to firebreath" FORCE)
set(FIREBREATH_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/FireBreath" CACHE INTERNAL "Path of fb projects" FORCE)
set(FB_PROJECTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/fb/projects" CACHE FILEPATH "root directory for fb projects" FORCE)
set(SWIG_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/swig" CACHE INTERNAL "Path of swig sources" FORCE) 


add_subdirectory(third_party)

add_subdirectory(src/swig)

# add_subdirectory(src/draft)

cmake_minimum_required(VERSION 2.6)

project(wsms)

set(CMAKE_CXX_COMPILER		"/usr/bin/clang++")
set(CMAKE_BUILD_TYPE		"Debug")
set(CMAKE_CXX_FLAGS		"-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG	"-g3")
set(CMAKE_CXX_FLAGS_RELEASE	"-O2 -DNDEBUG")

add_subdirectory(wsms)
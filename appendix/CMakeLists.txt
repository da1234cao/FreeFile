cmake_minimum_required (VERSION 3.8)

project ("deleteFile")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable (hanele "handle.cpp")
add_executable (deleteFile "deleteFile.cpp")
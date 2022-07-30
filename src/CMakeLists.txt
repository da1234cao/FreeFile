cmake_minimum_required (VERSION 3.8)

project ("freeFile")

set(CMAKE_CXX_STANDARD 17)

# boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} "../include")
link_directories(${Boost_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} "main.cpp")

# toml
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.1.0
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(tomlplusplus)
include_directories(${tomlplusplus_SOURCE_DIR}/include)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message(STATUS "compile with MSVC.set the VS_DEBUGGER_WORKING_DIRECTORY")
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY  "${EXECUTABLE_OUTPUT_PATH}")
endif()
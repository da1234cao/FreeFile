cmake_minimum_required (VERSION 3.8)

project ("freeFile")

# toml
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.1.0
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(tomlplusplus)
include_directories(${tomlplusplus_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus spdlog::spdlog)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message(STATUS "compile with MSVC.set the VS_DEBUGGER_WORKING_DIRECTORY")
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY  "${EXECUTABLE_OUTPUT_PATH}")
endif()
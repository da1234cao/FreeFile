cmake_minimum_required (VERSION 3.8)

project ("freeFile")

set(CMAKE_CXX_STANDARD 17)

# boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include(FetchContent)

#spdlog
## github访问较慢，使用gitee代替
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://gitee.com/moondaisy/spdlog
        GIT_TAG        v1.x
)
FetchContent_MakeAvailable(spdlog)
include_directories(${spdlog_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(appendix)
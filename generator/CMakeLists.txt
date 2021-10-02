cmake_minimum_required (VERSION 3.15)
project (RingLoading)

# The version number
set (RingLoading_VERSION_MAJOR 0)
set (RingLoading_VERSION_MINOR 1)

option(CXX "enable C++ compilation" ON)
if(CXX)
    enable_language(CXX)
endif()

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20 -Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -no-pie")

set( CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O3")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##### find Gurobi
if (NOT DEFINED ENV{GRB_LICENSE_FILE})
  message(STATUS "Gurobi license file variable not set")
endif()
find_package(GUROBI REQUIRED)

##### find boost
find_package(Boost COMPONENTS program_options REQUIRED)

##### project paths #####
set(LP_DIR Lp)
##########

##### project lib names #####
set(LP_LIB Lp)
##########

##### add project sub dirs #####
add_subdirectory(${LP_DIR})
##########

set(MAIN main)
add_executable(${MAIN} main.cpp)

target_include_directories(${MAIN} PRIVATE ${GUROBI_INCLUDE_DIRS} PRIVATE ${Boost_INCLUDE_DIR})
 #${Boost_INCLUDE_DIR}

target_link_libraries(${MAIN}
  ${LP_LIB}
  ${GUROBI_LIBRARY}
  ${GUROBI_CXX_LIBRARY}
  ${Boost_LIBRARIES}
  -pthread
)

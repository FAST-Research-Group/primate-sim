cmake_minimum_required(VERSION 3.10)
project(PrimSim)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Find Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options regex REQUIRED)

# Include directories for headers
include_directories(
    ${PrimSim_SOURCE_DIR}/inc
    ${PrimSim_SOURCE_DIR}/BFUs/inc
    ${PrimSim_SOURCE_DIR}/BFUProcessInstructionGen/inc
    ${PrimSim_SOURCE_DIR}/Experimental Generator
    ${Boost_INCLUDE_DIRS}
)

if(Boost_FOUND)
    # Add subdirectories for components
    add_subdirectory(${PrimSim_SOURCE_DIR}/src)
    add_subdirectory(${PrimSim_SOURCE_DIR}/BFUs/src)
    add_subdirectory(${PrimSim_SOURCE_DIR}/BFUProcessInstructionGen/src)
endif()

# Set compilation flags
set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

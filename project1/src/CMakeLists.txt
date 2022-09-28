#########
# Setup #
#########

# Minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Message
message(STATUS "spice file to MNA")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project's name
project (MNA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Werror=return-type -pthread -g")

#######################
# Include Directories #
#######################

#aux_source_directory(. CPP_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

###########
# Targets #
###########

# add the executable
add_executable(MNA main.cpp)


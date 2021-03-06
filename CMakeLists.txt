# Copyright (c) 2016, Technische Universität Dresden, Germany
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions
#    and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of
#    conditions and the following disclaimer in the documentation and/or other materials provided
#    with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its contributors may be used to
#    endorse or promote products derived from this software without specific prior written
#    permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.8)
project(phdeem)

option(BUILD_LIBRARY "Build the phdeem library" ON)
option(BUILD_EXAMPLES "Build the examples." OFF)
option(BUILD_TESTS "Build the test programs." OFF)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/common")

if(BUILD_LIBRARY)
    find_package(HDEEM REQUIRED)
    find_package(FreeIPMI REQUIRED)
    find_package(MPI REQUIRED)

    set(PHDEEM_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/phdeem.c" "${PROJECT_SOURCE_DIR}/src/phdeem.h")

    add_definitions("-Wall -Werror -pedantic -std=gnu99" ${MPI_C_COMPILE_FLAGS} ${MPI_C_LINK_FLAGS})
    include_directories(SYSTEM ${HDEEM_INCLUDE_DIRS} ${FreeIPMI_INCLUDE_DIRS} ${MPI_C_INCLUDE_PATH})
    add_library(${PROJECT_NAME} SHARED ${PHDEEM_SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${HDEEM_LIBRARIES} ${FreeIPMI_LIBRARIES} ${MPI_C_LIBRARIES})
endif()

if(BUILD_EXAMPLES)
    include_directories("src/" SYSTEM ${HDEEM_INCLUDE_DIRS} ${FreeIPMI_INCLUDE_DIRS} ${MPI_C_INCLUDE_PATH})
    add_executable("full_example" "examples/full_example.c")
    target_link_libraries("full_example" ${PROJECT_NAME})
endif()

if(BUILD_TESTS)
    add_executable("test_hash" "tests/test_hash.cpp")
endif()

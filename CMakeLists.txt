cmake_minimum_required(VERSION 3.10)

project(Composition VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

# bin directory
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

# Include directory
set(INCLUDE "${CMAKE_SOURCE_DIR}/include")

# Source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# composition library target
add_library(composition SHARED ${SOURCES})
target_include_directories(composition PRIVATE ${INCLUDE})

# basic_example target
add_executable(basic_example "${CMAKE_SOURCE_DIR}/examples/basic_example.cpp")
target_include_directories(basic_example PRIVATE ${INCLUDE})
target_link_libraries(basic_example composition)

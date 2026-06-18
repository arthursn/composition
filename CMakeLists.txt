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
target_include_directories(composition PUBLIC ${INCLUDE})

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_EXAMPLES)
  add_executable(basic_example "${CMAKE_SOURCE_DIR}/examples/basic_example.cpp")
  target_link_libraries(basic_example composition)
endif()

if(BUILD_TESTS)
  enable_testing()
  add_executable(test_composition
                 "${CMAKE_SOURCE_DIR}/tests/test_composition.cpp")
  target_link_libraries(test_composition composition)
  add_test(NAME test_composition COMMAND test_composition)
endif()

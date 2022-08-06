cmake_minimum_required(VERSION 3.10)

project(composition VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

# Vendor files
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(VENDOR_INCLUDE "${VENDOR_DIR}/include")
file(GLOB VENDOR_SOURCES "${VENDOR_DIR}/src/*.cpp")

# Source files
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# composition target
add_executable(composition ${SOURCES} ${VENDOR_SOURCES})
target_include_directories(composition PRIVATE ${SRC_DIR} ${VENDOR_INCLUDE})
target_link_libraries(composition ${TQ_SO} dl pthread)
set_target_properties(composition PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

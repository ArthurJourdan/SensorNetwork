
cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE DEBUG)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(SENSOR_NETWORK sensor_network)


project(${SENSOR_NETWORK} C)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

find_package(MPI REQUIRED)
find_package(Threads REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${SRC_DIR}/*.c)

add_executable(${SENSOR_NETWORK} ${SRCS})

target_include_directories(${SENSOR_NETWORK} PRIVATE ${INCLUDE_DIR} ${MPI_C_INCLUDE_DIRS})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(${SENSOR_NETWORK} PRIVATE -Wall -Wextra -D DEBUG)
endif ()


target_link_libraries(${SENSOR_NETWORK} ${MPI_C_LIBRARIES} Threads::Threads)
target_link_libraries(${SENSOR_NETWORK} m)
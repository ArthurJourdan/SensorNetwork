
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

file(GLOB SRCS CONFIGURE_DEPENDS ${SRC_DIR}/*.c)

# set(SRCS
#     ${SRC_DIR}main.c
#     ${SRC_DIR}grid_setting.c
#     ${SRC_DIR}mpi_utils.c
#     ${SRC_DIR}sensor.c
#     ${SRC_DIR}utils.c)

add_executable(${SENSOR_NETWORK} ${SRCS})

add_compile_options(-Wall -Wextra -pedantic -Werror)

target_link_libraries(${SENSOR_NETWORK} ${MPI_C_LIBRARIES} Threads::Threads)
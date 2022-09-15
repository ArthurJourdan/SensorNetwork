
cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE DEBUG)
set(SENSOR_NETWORK sensor_network)


project(
        ${SENSOR_NETWORK}
        VERSION 0.1
        LANGUAGES C)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories(${INCLUDE_DIR})

set(SRCS
    ${SRC_DIR}main.c
    ${SRC_DIR}grid_setting.c
    ${SRC_DIR}mpi_utils.c
    ${SRC_DIR}sensor.c
    ${SRC_DIR}utils.c)

add_executable(${SENSOR_NETWORK} ${SRCS})

target_link_libraries(${SENSOR_NETWORK} ${MPI_C_LIBRARIES})
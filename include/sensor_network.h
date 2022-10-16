/**
 * @file sensor_network.h
 * @author Arthur Jourdan
 * @date of creation 13/10/22.
 * @brief constants and structure declaration
 */

#ifndef MAKEFILE_SENSOR_NETWORK_H
#define MAKEFILE_SENSOR_NETWORK_H

#include <mpi.h>

#define NB_DIMENSIONS 2
#define NB_NEIGHBOURS (NB_DIMENSIONS * 2)

#define DATA_PACK_SIZE ((sizeof(int) * 9) + (sizeof(float) * NB_DIMENSIONS) + sizeof(float) + sizeof(float))
// size of elements in struct tm, sensor position on grid, magnitude, depth,

typedef struct
{
    struct tm *timestamp;
    float coordinates[NB_DIMENSIONS];
    float magnitude;
    float depth;
} sensor_reading_t;
// This reading consists of date, time,
//     latitude and longitude of the earthquake point, magnitude, and depth of the quake from the sensor.
//  You can generate random float values to simulate the sensor readings and you can refer to this link as
//  a reference. The following table lists sample readings (simulated at a cycle of 10 seconds):

typedef struct grid
{
    int size[NB_DIMENSIONS];
    int process_position[NB_DIMENSIONS];
    int neighbours[NB_NEIGHBOURS];
    MPI_Comm comm;
    MPI_Request pending_recv[NB_NEIGHBOURS * 2];
    char recv_bufs[NB_NEIGHBOURS * 2][DATA_PACK_SIZE];
    sensor_reading_t *data_history;
    unsigned int data_history_size;
} grid_t;

// #define DATA_PACK_ELEM_NB (9 + NB_DIMENSIONS + 1 + 1)

#endif // MAKEFILE_SENSOR_NETWORK_H
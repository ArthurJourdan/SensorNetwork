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

enum tags
{
    TAG_SENSOR_TO_SENSOR = 1,
    TAG_SENSOR_TO_BASE,
    TAG_SENSOR_TO_BASE_ALIVE,
    TAG_BASE_TO_SENSOR_EXIT,
};

// probability of an error occurring (1 / ERROR_PROBABILITY)
#define ERROR_PROBABILITY 500

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

#endif // MAKEFILE_SENSOR_NETWORK_H
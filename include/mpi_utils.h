/**
 * @file mpi_utils.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_MPI_UTILS_H
#define SENSORNETWORK_MPI_UTILS_H

#include "sensor_network.h"

#include <mpi.h>
#include <stdbool.h>

typedef struct
{
    int rank;
    int nb_processes;
    // TODO add communicator ?
    // TODO add coordinates ?
} mpi_info_t;

// Get
bool get_comm_info(MPI_Comm comm, mpi_info_t *process, int coordinates[NB_DIMENSIONS]);
bool get_neighbours(MPI_Comm comm, int neighbours[NB_NEIGHBOURS]);

// Send
bool send_neighbours(MPI_Comm comm, const int neighbours[NB_NEIGHBOURS], char buf[DATA_PACK_SIZE], int count);

// print
void print_MPI_error(MPI_Status *status);
void print_coordinates(const int coordinates[NB_DIMENSIONS]);
void print_float_coordinates(const float coordinates[NB_DIMENSIONS]);

#endif // SENSORNETWORK_MPI_UTILS_H
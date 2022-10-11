/**
 * @file mpi_utils.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_MPI_UTILS_H
#define SENSORNETWORK_MPI_UTILS_H

#include <mpi.h>
#include <stdbool.h>

typedef struct
{
    int rank;
    int nb_processes;
    // TODO add communicator ?
    // TODO add coordinates ?
} mpi_info_t;

bool get_comm_info(MPI_Comm comm, mpi_info_t *process, int coordinates[]);

void print_coordinates(const int coordinates[]);

#endif // SENSORNETWORK_MPI_UTILS_H
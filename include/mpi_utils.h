/**
 * @file mpi_utils.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_MPI_UTILS_H
#define SENSORNETWORK_MPI_UTILS_H

typedef struct
{
    int rank;
    int nb_processes;
} mpi_info_t;

void init_MPI(int *ac, char **av[], mpi_info_t *process);

#endif // SENSORNETWORK_MPI_UTILS_H
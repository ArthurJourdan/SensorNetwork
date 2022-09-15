/**
 * @file mpi_utils.c
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include <mpi.h>
#include <stdbool.h>

#include "mpi_utils.h"

void init_MPI(int *ac, char **av[], mpi_info_t *process)
{
    static bool init = false;

    if (!init) {
        MPI_Init(ac, av);
        init = true;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &process->rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process->nb_processes);
}
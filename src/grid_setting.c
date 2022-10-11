/**
 * @file grid_settings.c
 * @author Arthur Jourdan
 * @date of creation 24/09/22.
 * @brief TODO
 */

#include "grid.h"
#include "mpi_utils.h"

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>

bool get_grid_size(grid_t *grid)
{
    printf("Enter grid size (one dimension per line)\n");
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (scanf("%u", &grid->size[i]) != 1) {
            return false;
        }
    }
    return true;
}

/**
 * @brief
 * @param process_grid [in]
 * @param new_comm [out]
 * @param rank [out]
 * @param nb_processes [out]
 * @return True if no error occurred, false otherwise
 */
bool init_grid(MPI_Comm old_comm, MPI_Comm *new_comm, mpi_info_t *process, grid_t *grid)
{
    int wrap_around[NB_DIMENSIONS] = {false};
    bool reorder = true;
    MPI_Comm tmp_comm;
    const int color = (process->rank ? 1 : 0);

    if (MPI_Comm_split(old_comm, color, process->rank, &tmp_comm) != MPI_SUCCESS) {
        return false;
    }
    if (color) {
        if (MPI_Cart_create(tmp_comm, NB_DIMENSIONS, grid->size, wrap_around, reorder, new_comm) != MPI_SUCCESS) {
            return false;
        }
        return get_comm_info(*new_comm, process, grid->process_position);
    }
    return get_comm_info(MPI_COMM_WORLD, process, grid->process_position);
}
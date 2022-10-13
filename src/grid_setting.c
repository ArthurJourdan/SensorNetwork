/**
 * @file grid_settings.c
 * @author Arthur Jourdan
 * @date of creation 24/09/22.
 * @brief TODO
 */

#include "grid.h"
#include "mpi_utils.h"

#include <unistd.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Get grid's size with user input
 * @param grid out
 * @return true if filled grid size, false if error occurred
 */
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
 * @brief Create cartesian topology of 2d array, excluding the master process from it
 * @param process_grid [in]
 * @param new_comm [out]
 * @param rank [out]
 * @param nb_processes [out]
 * @return True if no error occurred, false otherwise
 */
static bool init_grid_topology(MPI_Comm old_comm, MPI_Comm *new_comm, mpi_info_t *process, grid_t *grid)
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
        if (!get_comm_info(*new_comm, process, grid->process_position))
            return false;
#ifdef DEBUG
        printf("neighbours of %i :\n", process->rank);
#endif
        return get_neighbours(grid->comm, grid->neighbours);
    }
    grid->comm = MPI_COMM_WORLD;
    for (unsigned int i = 0; i < NB_NEIGHBOURS; ++i) {
        grid->neighbours[i] = MPI_PROC_NULL;
    }
    return get_comm_info(MPI_COMM_WORLD, process, grid->process_position);
}

bool init_grid(mpi_info_t *process, grid_t *grid)
{
    int total_grid_elements = 1;

    if (process->rank == 0) {
        get_grid_size(grid); // get grid size from user input
    }
    MPI_Bcast(grid->size, NB_DIMENSIONS, MPI_UNSIGNED, 0, MPI_COMM_WORLD); // send grid size to every process
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        total_grid_elements *= grid->size[i];
    }
    if (total_grid_elements != process->nb_processes - 1) {
        if (process->rank == 0) {
            dprintf(STDERR_FILENO,
                "Input grid size (%u) is different from number of MPI processes (%i withouth the master)\n",
                total_grid_elements,
                process->nb_processes - 1);
        }
        return false;
    }
    if (!init_grid_topology(MPI_COMM_WORLD, &grid->comm, process, grid))
        return false;
    return true;
}
/**
 * @file grid.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include "utils.h"
#include "mpi_utils.h"
#include "grid.h"

#include <stdbool.h>
#include <mpi.h>

#define NB_DIMENSIONS 2

typedef struct grid
{
    int size[NB_DIMENSIONS];
    int process_position[NB_DIMENSIONS];
    MPI_Comm communicator;
} grid_t;

bool get_grid_size(grid_t *grid);

bool init_grid(MPI_Comm old_comm, MPI_Comm *new_comm, mpi_info_t *process, grid_t *grid);

#endif // PROJECT_GRID_H
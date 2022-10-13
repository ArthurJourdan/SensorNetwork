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
#include "sensor_network.h"

#include <stdbool.h>
#include <mpi.h>

bool get_grid_size(grid_t *grid);

bool init_grid(mpi_info_t *process, grid_t *grid);

#endif // PROJECT_GRID_H
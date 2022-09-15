/**
 * @file grid_setting.c
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include <stdio.h>
#include <stdbool.h>

#include "grid.h"

bool get_grid_size(u_vector_t *grid_size)
{
    printf("Enter grid size\n");
    printf("M :");
    fflush(stdout);
    if (scanf("%u", &grid_size->x) != 1)
        return false;
    printf("N :");
    fflush(stdout);
    if (scanf("%u", &grid_size->x) != 1)
        return false;
    return true;
}

// todo  bool set_grid_shape(grid_t *grid);
// todo  bool get_grid_position(grid_t *grid)
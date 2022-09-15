/**
 * @file grid.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include <stdbool.h>

#include "utils.h"

typedef struct
{
    u_vector_t size;
    u_vector_t process_position;
} grid_t;

bool get_grid_size(u_vector_t *grid_size);

#endif // PROJECT_GRID_H
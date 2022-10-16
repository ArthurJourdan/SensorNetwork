/**
 * @file sensor_error.c
 * @author Arthur Jourdan
 * @date of creation 16/10/22.
 * @brief TODO
 */

#include "utils.h"
#include "mpi.h"

/**
 * @brief tell if error must be generated
 * @return true if random error, false otherwise
 */
bool random_error(void)
{
    int rand_val = rand_int(0, 2);

    if (!rand_val)
        return true;
    return false;
}
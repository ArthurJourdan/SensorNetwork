/**
 * @file sensor_error.c
 * @author Arthur Jourdan
 * @date of creation 16/10/22.
 * @brief TODO
 */

#include "utils.h"

/**
 * @brief tell if error must be generated
 * @return true if random error, false otherwise
 */
bool random_error(void)
{
    int rand_val;
#ifdef ERROR_PROBABILITY
    rand_val = rand_int(0, ERROR_PROBABILITY);
#else
    rand_val = rand_int(0, 100);
#endif

    if (!rand_val)
        return true;
    return false;
}
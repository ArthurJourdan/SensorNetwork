/**
 * @file utils.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief Some useful functions
 */

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

float rand_float(const float max)
{
    return ((float) rand() / (float) (RAND_MAX)) * max;
}

bool convert_time(const time_t *in, struct tm **out)
{
    // todo free(out) ?
    *out = gmtime(in);
    if (!(*out))
        return false;
    return true;
}
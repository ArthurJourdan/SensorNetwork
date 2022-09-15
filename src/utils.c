/**
 * @file utils.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief Some useful functions
 */

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void init_rand(const unsigned int additional_seed)
// todo pass rank as add seed to make each process random
{
    srand(time(NULL) + additional_seed);
}

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
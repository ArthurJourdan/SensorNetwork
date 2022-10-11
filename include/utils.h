/**
 * @file utils.h
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_UTILS_H
#define SENSORNETWORK_UTILS_H

#include <time.h>
#include <stdbool.h>

typedef struct
{
    unsigned int x;
    unsigned int y;
} u_vector_t;

typedef struct
{
    float x;
    float y;
} f_vector_t;

float rand_float(const float max);

bool convert_time(const time_t *in, struct tm **out);

#endif // SENSORNETWORK_UTILS_H
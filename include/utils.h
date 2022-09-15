/**
 * @file utils.h
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_UTILS_H
#define SENSORNETWORK_UTILS_H

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
void init_rand(unsigned int additional_seed);

#endif // SENSORNETWORK_UTILS_H
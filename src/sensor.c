/**
 * @file sensor.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#include "utils.h"
#include "sensor.h"

void read_data(sensor_reading_t *out)
{
    convert_time(time(NULL), &out->timestamp);
    // todo coordinates
    out->magnitude = rand_float(9.5 /*max magnitude*/);
    out->depth = rand_float(700);
}
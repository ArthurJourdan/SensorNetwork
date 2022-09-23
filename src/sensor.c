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
    const time_t act_time = time(NULL);
    convert_time(&act_time, &out->timestamp);
    // todo coordinates
    out->magnitude = rand_float(9.5f /*max magnitude*/);
    out->depth = rand_float(700.f);
}
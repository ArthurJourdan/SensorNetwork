/**
 * @file sensor.h
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_SENSOR_H
#define SENSORNETWORK_SENSOR_H

#include <time.h>

#include "utils.h"

typedef struct
{
    struct tm *timestamp;
    f_vector_t coordinates;
    float magnitude;
    float depth;

} sensor_reading_t;
// This reading consists of date, time,
//     latitude and longitude of the earthquake point, magnitude, and depth of the quake from the sensor.
//  You can generate random float values to simulate the sensor readings and you can refer to this link as
//  a reference. The following table lists sample readings (simulated at a cycle of 10 seconds):

#endif // SENSORNETWORK_SENSOR_H
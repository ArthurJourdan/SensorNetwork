#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "sensor.h"
#include "base_station.h"
#include "utils.h"

#define BALLOON_CYCLE 500000 // balloon sensor cycle time in microseconds

sensor_reading_t balloon_buffer;

void balloon_thread(__attribute__((unused)) void *ptr)
{
    pthread_mutex_lock(&lock);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&lock);

        /* This thread periodically produces seismic readings. This reading consists of date, time, latitude and
        longitude of the earthquake point, magnitude, and depth of the quake from the sensor. However, the
        generated magnitude always exceeds the predefined threshold (e.g., magnitude > 2.5) */
        sensor_reading_t sensor_data;

        read_data(NULL, NULL, &sensor_data);

        // sensor_data.timestamp =

        pthread_mutex_lock(&lock);
        copy_sensor_data(&sensor_data, &balloon_buffer);
        pthread_mutex_unlock(&lock);

        usleep(BALLOON_CYCLE);
        pthread_mutex_lock(&lock);
    }
    pthread_mutex_unlock(&lock);

#ifdef DEBUG
    printf("Balloon thread has exited\n");
#endif
}
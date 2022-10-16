#include <unistd.h>
#include <pthread.h>

#include "sensor.h"
#include "base_station.h"
#include "utils.h"

#define BALLOON_CYCLE 0.2 // balloon sensor cycle time in seconds

sensor_reading_t balloon_buffer[BALLOON_BUFFER_SIZE];
unsigned int balloon_index = 0; // index used for ballon_buffer

// todo use read_data function
static void read_balloon_data(sensor_reading_t *out)
{
    const time_t act_time = time(NULL);
    convert_time(&act_time, &out->timestamp);
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        out->coordinates[i] = 0;
    }
    out->magnitude = rand_float(0, 9.5f /*max magnitude*/);
    out->depth = rand_float(0, 700.f);
}

void balloon_thread(void *ptr)
{
    pthread_mutex_lock(&MUTEX_EXIT);

    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&MUTEX_EXIT);

        /* This thread periodically produces seismic readings. This reading consists of date, time, latitude and
        longitude of the earthquake point, magnitude, and depth of the quake from the sensor. However, the
        generated magnitude always exceeds the predefined threshold (e.g., magnitude > 2.5) */
        sensor_reading_t sensor_data;

        read_balloon_data(&sensor_data);

        // sensor_data.timestamp =

        balloon_buffer[balloon_index] = sensor_data;

        if (++balloon_index >= BALLOON_BUFFER_SIZE)
            balloon_index = 0;
        pthread_mutex_lock(&MUTEX_EXIT);

        sleep(BALLOON_CYCLE);
    }
    pthread_mutex_unlock(&MUTEX_EXIT);
}
#include <unistd.h>
#include <pthread.h>

#include "balloon.h"
#include "sensor.h"
#include "base_station.h"
#include "utils.h"

#define BALLOON_CYCLE 0.2 // balloon sensor cycle time in seconds

sensor_reading_t balloon_buffer[BALLOON_BUFFER_SIZE];
unsigned int balloon_index = 0; // index used for ballon_buffer

static void read_balloon_data(sensor_reading_t *out)
{
    const time_t act_time = time(NULL);
    convert_time(&act_time, &out->timestamp);
    // todo coordinates
    out->magnitude = rand_float(9.5f /*max magnitude*/);
    out->depth = rand_float(700.f);
}

void balloon_thread(void *ptr)
{
    while (!THREADS_EXIT) {
        /* This thread periodically produces seismic readings. This reading consists of date, time, latitude and
        longitude of the earthquake point, magnitude, and depth of the quake from the sensor. However, the
        generated magnitude always exceeds the predefined threshold (e.g., magnitude > 2.5) */
        sensor_reading_t sensor_data;

        read_balloon_data(&sensor_data);

        // sensor_data.timestamp =

        balloon_buffer[balloon_index] = sensor_data;

        if (++balloon_index >= BALLOON_BUFFER_SIZE)
            balloon_index = 0;

        sleep(BALLOON_CYCLE);
    }
}

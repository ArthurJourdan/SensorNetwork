#ifndef BASE_STATION_H
#define BASE_STATION_H

#include <pthread.h>
#include "mpi_utils.h"
#include "sensor.h"

#define STR_EXIT            "SIGQUIT"
#define STR_ALIVE           "IMALIVE"
#define BALLOON_BUFFER_SIZE 10
#define POLLING_RATE        1 // seconds

void base_station(mpi_info_t process);
void balloon_thread(void *ptr);
void reading_thread(void *ptr);
void fault_detection_thread(void *param);

extern int THREADS_EXIT;
extern sensor_reading_t balloon_buffer;

extern pthread_mutex_t lock;

#endif
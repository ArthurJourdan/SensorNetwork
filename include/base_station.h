#ifndef BASE_STATION_H
#define BASE_STATION_H

#include <pthread.h>
#include "mpi_utils.h"
#include "sensor.h"

#define STR_EXIT            "SIGQUIT"
#define BALLOON_BUFFER_SIZE 10
#define POLLING_RATE        1 // seconds

void base_station(mpi_info_t process);
void balloon_thread(void *ptr);
void reading_thread(void *ptr);

extern int THREADS_EXIT;
extern pthread_mutex_t MUTEX_EXIT;
extern sensor_reading_t balloon_buffer[BALLOON_BUFFER_SIZE];
extern unsigned int balloon_index;

#endif
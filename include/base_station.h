#ifndef BASE_STATION_H
#define BASE_STATION_H

#include <pthread.h>
#include "mpi_utils.h"

void base_station(mpi_info_t process);

extern int THREADS_EXIT;
extern pthread_mutex_t MUTEX_EXIT;

#endif
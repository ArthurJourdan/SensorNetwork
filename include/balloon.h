#ifndef BALLOON_H
#define BALLOON_H

#include "sensor.h"

#define BALLOON_BUFFER_SIZE 10

void balloon_thread(void *ptr);

extern sensor_reading_t balloon_buffer[BALLOON_BUFFER_SIZE];
extern unsigned int balloon_index;

#endif
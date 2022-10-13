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
#include "grid.h"
#include "sensor_network.h"

// Print
void print_data(sensor_reading_t *data);

// Get
void read_data(sensor_reading_t *data);
bool read_pack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE]);
void unpack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data);

// Set
bool read_send_data_neighbours(mpi_info_t *process, grid_t *grid);

bool create_sensor_data_type(MPI_Datatype *new_data_type);

#endif // SENSORNETWORK_SENSOR_H
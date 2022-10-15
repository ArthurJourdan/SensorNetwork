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

bool launch_sensor(grid_t *grid);

// Print
void print_data(sensor_reading_t *data);

// Get
bool generate_geo_coordinates(grid_t *grid, float *latitude, float *longitude);
void read_data(grid_t *grid, sensor_reading_t *data);
bool pack_data(grid_t *grid, sensor_reading_t *data, char packed_data[DATA_PACK_SIZE]);
bool unpack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data);

float distance(float lat1, float lon1, float lat2, float lon2, char unit);
bool check_neighbour_data(grid_t *grid);
bool neighbour_data_cmp(grid_t *grid, const int neighbour_req_index, const bool respond);

// Set
bool read_send_data_neighbours(grid_t *grid);
bool init_neighbour_recv(grid_t *grid, const unsigned int neighbour_req_index);
bool init_all_neighbour_recv(grid_t *grid);

#endif // SENSORNETWORK_SENSOR_H
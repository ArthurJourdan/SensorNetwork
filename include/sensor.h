/**
 * @file sensor.h
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#ifndef SENSORNETWORK_SENSOR_H
#define SENSORNETWORK_SENSOR_H

#include <time.h>
#include <stdbool.h>

#include "utils.h"
#include "grid.h"
#include "sensor_network.h"

bool launch_sensor(grid_t *grid);

// Print
void print_data(sensor_reading_t *data);
void dprint_data_one_liner(const int fd, sensor_reading_t *data);
void dprint_data_array(const int fd, sensor_reading_t *data_array, const unsigned int size, bool match);

// Get
bool generate_geo_coordinates(
    int grid_size[NB_DIMENSIONS], int process_position[NB_DIMENSIONS], float *latitude, float *longitude);
void read_data(int grid_size[NB_DIMENSIONS], int process_position[NB_DIMENSIONS], sensor_reading_t *data);
bool pack_data(MPI_Comm comm, sensor_reading_t *data, char packed_data[DATA_PACK_SIZE]);
bool unpack_data(MPI_Comm comm, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data);

float distance(float lat1, float lon1, float lat2, float lon2, char unit);
bool check_neighbour_data(grid_t *grid);
bool sensor_data_cmp(sensor_reading_t *first, sensor_reading_t *second);
bool neighbour_data_cmp(grid_t *grid, const int neighbour_req_index);

// Set
bool init_neighbour_recv(grid_t *grid, const unsigned int neighbour_req_index);
bool init_all_neighbour_recv(grid_t *grid);

bool read_send_data_neighbours(grid_t *grid);

bool save_data_in_history(sensor_reading_t **history, unsigned int *history_size, sensor_reading_t *data);

// Finish
bool finish_all_neighbour_recv(grid_t *grid);

#endif // SENSORNETWORK_SENSOR_H
/**
 * @file sensor_data_cmp.c
 * @author Arthur Jourdan
 * @date of creation 14/10/22.
 * @brief TODO
 */

#include <stdbool.h>
#include <stdio.h>

#include "grid.h"
#include "sensor.h"

/**
 * @brief
 * @param needle [in] Pointer
 * @param haystack [out] Array
 * @param haystack_size [in] number of elements in the haystack array
 * @return -1 if did not find the timestamp, index otherwise
 */
static int find_data_from_time(sensor_reading_t *needle, sensor_reading_t *haystack, const unsigned int haystack_size)
{
    for (int i = 0; i < (int) haystack_size; ++i) {
        if (needle->timestamp == haystack[i].timestamp)
            return i;
    }
    return -1;
}

static bool data_cmp(sensor_reading_t *first, sensor_reading_t *second)
{
    const float max_coherant_distance = 500; // in Kilometers
    float dist_2_points;                     // in Kilometers

    if (first->magnitude <= 2.5 || second->magnitude <= 2.5)
        return false;
    if (NB_DIMENSIONS == 2) {
        dist_2_points =
            distance(first->coordinates[0], first->coordinates[1], second->coordinates[0], second->coordinates[1], 'K');
#ifdef DEBUG
        printf("Distance between [%f, %f] and [%f, %f]: %fkm\n",
            first->coordinates[0],
            first->coordinates[1],
            second->coordinates[0],
            second->coordinates[1],
            dist_2_points);
#endif
    } else {
        for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
            if (first->coordinates[i] != second->coordinates[i])
                return false;
        }
    }
    if (dist_2_points < max_coherant_distance)
        return true;
    return false;
}

bool neighbour_data_cmp(grid_t *grid, const int neighbour_req_index)
{
    sensor_reading_t neighbour_data;
    sensor_reading_t *my_data;
    char packed_data[2][DATA_PACK_SIZE];
    int my_data_index;

    unpack_data(grid->comm, grid->recv_bufs[neighbour_req_index], &neighbour_data);
    my_data_index = find_data_from_time(&neighbour_data, grid->data_history, grid->data_history_size);
    if (my_data_index == -1) {
        return false;
    }
    my_data = &grid->data_history[my_data_index];
    if (data_cmp(my_data, &neighbour_data)) {
        pack_data(MPI_COMM_WORLD, my_data, packed_data[0]);
        pack_data(MPI_COMM_WORLD, &neighbour_data, packed_data[1]);
        // send to base station
        MPI_Send(packed_data[0], DATA_PACK_SIZE, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
        MPI_Send(packed_data[1], DATA_PACK_SIZE, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
    }
    return true;
}
/**
 * @file sensor_data_cmp.c
 * @author Arthur Jourdan
 * @date of creation 14/10/22.
 * @brief TODO
 */

#include <stdbool.h>

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

bool neighbour_data_cmp(grid_t *grid, const int neighbour_req_index, const bool respond)
{
    sensor_reading_t neighbour_data;
    int my_data_index;
    unpack_data(grid, grid->recv_bufs[neighbour_req_index], &neighbour_data);

    if (NB_DIMENSIONS < 2)
        return false;
    my_data_index = find_data_from_time(&neighbour_data, grid->data_history, grid->data_history_size);
    if (my_data_index == -1)
        return false;
    float remoteness = distance(neighbour_data.coordinates[0],
        neighbour_data.coordinates[1],
        grid->data_history[my_data_index].coordinates[0],
        grid->data_history[my_data_index].coordinates[1],
        'K');
    printf("remoteness : %f.\n", remoteness); // todo test
    init_neighbour_recv(grid, neighbour_req_index);
    // todo send to base station if cmp is coherent
    return true;
}
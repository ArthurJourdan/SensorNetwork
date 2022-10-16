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

static bool check_empty_data(sensor_reading_t *data)
{
    char *tmp = (char *) data;
    for (unsigned int i = 0; i < DATA_PACK_SIZE; ++i) {
        if (tmp[i])
            return true;
    }
    return false;
}

static bool data_cmp(sensor_reading_t *first, sensor_reading_t *second)
{
    float remoteness;
    printf("data_cmp.\n");
    if (first->magnitude <= 2.5 || second->magnitude <= 2.5)
        return false;
    if (NB_DIMENSIONS == 2) {
        remoteness =
            distance(first->coordinates[0], first->coordinates[1], second->coordinates[0], second->coordinates[1], 'K');
        printf("remoteness : %f.\n", remoteness); // todo test
    } else {
        for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
            if (first->coordinates[i] != second->coordinates[i])
                return false;
        }
    }
    return true;
}

bool neighbour_data_cmp(grid_t *grid, const int neighbour_req_index)
{
    sensor_reading_t neighbour_data;
    sensor_reading_t *my_data;
    int my_data_index;

    unpack_data(grid, grid->recv_bufs[neighbour_req_index], &neighbour_data);
    my_data_index = find_data_from_time(&neighbour_data, grid->data_history, grid->data_history_size);
    if (my_data_index == -1)
        return false;
    my_data = &grid->data_history[my_data_index];
    if (check_empty_data(my_data))
        return false;
    if (data_cmp(my_data, &neighbour_data))
        printf("%s.\n", "sending to base station");
    ; // send to base station
    return true;
}
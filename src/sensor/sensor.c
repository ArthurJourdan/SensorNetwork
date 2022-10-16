/**
 * @file sensor.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#include "sensor.h"

#include <time.h>
#include <stdio.h>

bool launch_sensor(grid_t *grid)
{
    time_t act_time = time(NULL);

    init_all_neighbour_recv(grid);
    while (true /* todo set condition or a way to exit */) {
        while (act_time >= time(NULL)) {};
        act_time = time(NULL);
        read_send_data_neighbours(grid);
        check_neighbour_data(grid);
    }
    return true;
}
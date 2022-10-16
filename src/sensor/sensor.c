/**
 * @file sensor.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#include "sensor.h"
#include "mpi_utils.h"
#include "base_station.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

bool is_exit(void)
{
    int flag = 0;
    MPI_Status status = {0};
    char buffer[10] = {0};

    if (MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status) != MPI_SUCCESS)
        return false;
    if (!flag)
        return false;
    if (status.MPI_ERROR != MPI_SUCCESS) {
        print_MPI_error(&status);
        return false;
    }
    MPI_Recv(buffer, strlen(STR_EXIT), MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (!strcmp(STR_EXIT, buffer))
        return true;
    return false;
}

bool launch_sensor(grid_t *grid)
{
    time_t act_time = time(NULL);

    init_all_neighbour_recv(grid);
    while (true /* todo set condition or a way to exit */) {
        while (act_time >= time(NULL)) {};
        act_time = time(NULL);
        read_send_data_neighbours(grid);
        check_neighbour_data(grid);
        // check end
    }
    return true;
}
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
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static bool is_end(void)
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

void sig_handler()
{
    ;
}

bool launch_sensor(grid_t *grid)
{
    time_t act_time = time(NULL);
    MPI_Request request;

    signal(SIGQUIT, sig_handler);
    init_all_neighbour_recv(grid);
    while (!is_end()) {
        while (act_time >= time(NULL)) {};
        act_time = time(NULL);
        read_send_data_neighbours(grid);
        check_neighbour_data(grid);
        if (random_error()) {
            dprintf(STDERR_FILENO, "Exiting - Error occurred in sensor at coordinates ");
            print_coordinates(grid->process_position);
            break;
        } else {
            MPI_Isend(STR_ALIVE, strlen(STR_ALIVE), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request);
        }
    }
    finish_all_neighbour_recv(grid);
    return true;
}
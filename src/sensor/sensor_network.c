/**
 * @file sensor_network.c
 * @author Arthur Jourdan
 * @date of creation 15/09/22.
 * @brief TODO
 */

#include <mpi.h>
#include <stdbool.h>

#include "sensor.h"
#include "mpi_utils.h"

bool read_send_data_neighbours(grid_t *grid)
{
    char packed_data[DATA_PACK_SIZE] = {0};
    char recv_packed_data[NB_NEIGHBOURS][DATA_PACK_SIZE] = {{0}};
    sensor_reading_t recv_data[NB_NEIGHBOURS] = {{0}};

    if (!read_pack_data(grid, packed_data))
        return false;
    if (!send_recv_neighbours(grid->comm, grid->neighbours, packed_data, DATA_PACK_SIZE, recv_packed_data))
        return false;
    for (unsigned int i = 0; i < NB_NEIGHBOURS; ++i) {
        if (grid->neighbours[i] == MPI_PROC_NULL)
            continue;
        unpack_data(grid, recv_packed_data[i], &(recv_data[i]));
        print_data(&(recv_data[i]));
    };
    return true;
}
/**
 * @file sensor_network.c
 * @author Arthur Jourdan
 * @date of creation 15/09/22.
 * @brief TODO
 */

#include <mpi.h>
#include <stdbool.h>
#include <string.h>

#include "sensor.h"
#include "mpi_utils.h"

bool init_neighbour_recv(grid_t *grid, const unsigned int neighbour_req_index)
{
    if (grid->neighbours[(neighbour_req_index / 2)] == MPI_PROC_NULL)
        return false;
    memset(grid->recv_bufs[neighbour_req_index], 0, sizeof(char) * (DATA_PACK_SIZE));
    if (MPI_Irecv(grid->recv_bufs[neighbour_req_index],
            DATA_PACK_SIZE,
            MPI_PACKED,
            grid->neighbours[neighbour_req_index / 2],
            TAG_SENSOR_TO_SENSOR,
            grid->comm,
            &grid->pending_recv[neighbour_req_index])
        != MPI_SUCCESS)
        return false;
    return true;
}

bool init_all_neighbour_recv(grid_t *grid)
{
    bool retval = true;

    for (unsigned int i = 0; i < NB_NEIGHBOURS * 2; ++i) {
        if (!init_neighbour_recv(grid, i))
            retval = false;
    }
    return retval;
}

bool finish_all_neighbour_recv(grid_t *grid)
{
    for (unsigned int i = 0; i < NB_NEIGHBOURS * 2; ++i) {
        if (grid->neighbours[(i / 2)] == MPI_PROC_NULL)
            return false;
        MPI_Cancel(&grid->pending_recv[i]);
    }
    return true;
}

/**
 * @brief check if neighbour has sent a data
 * @param grid
 * @return
 */
bool check_neighbour_data(grid_t *grid)
{
    int flag = 0;
    MPI_Status status = {0};

    for (unsigned int i = 0; i < NB_NEIGHBOURS * 2; ++i) {
        if (grid->neighbours[(i / 2)] == MPI_PROC_NULL)
            continue;
        flag = 0;
        MPI_Iprobe(grid->neighbours[(i / 2)], TAG_SENSOR_TO_SENSOR, grid->comm, &flag, &status);
        if (!flag)
            continue;
        neighbour_data_cmp(grid, (int) i);
        init_neighbour_recv(grid, i);
    }
    return true;
}

bool read_send_data_neighbours(grid_t *grid)
{
    sensor_reading_t data = {0};
    char packed_data[DATA_PACK_SIZE] = {0};

    read_data(grid->size, grid->process_position, &data);
    if (data.magnitude <= 2.5f)
        return true;
    save_data_in_history(&grid->data_history, &grid->data_history_size, &data);
    if (!pack_data(grid->comm, &data, packed_data))
        return false;
    if (!send_neighbours(grid->comm, grid->neighbours, packed_data, DATA_PACK_SIZE))
        return false;
    return true;
}
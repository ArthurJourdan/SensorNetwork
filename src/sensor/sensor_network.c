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

bool init_neighbour_recv(grid_t *grid, const unsigned int neighbour_req_index)
{
    if (grid->neighbours[(neighbour_req_index / 2)] == MPI_PROC_NULL)
        return false;
    if (MPI_Irecv(grid->recv_bufs[neighbour_req_index],
            DATA_PACK_SIZE,
            MPI_PACKED,
            grid->neighbours[neighbour_req_index / 2],
            0,
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
        init_neighbour_recv(grid, i);
    }
    return retval;
}

/**
 * @brief check if neighbour has sent a data
 * @param grid
 * @return
 */
bool check_neighbour_data(grid_t *grid)
{
    int flag = 0;
    MPI_Status status;

    for (unsigned int i = 0; i < NB_NEIGHBOURS * 2; ++i) {
        if (grid->neighbours[(i / 2)] == MPI_PROC_NULL)
            continue;
        flag = 0;
        if (MPI_Request_get_status(grid->pending_recv[i], &flag, &status) != MPI_SUCCESS)
            continue;
        if (!flag) // operation is not complete
            continue;
        if (status.MPI_ERROR != MPI_SUCCESS) {
            print_MPI_error(&status);
            continue;
        }
        neighbour_data_cmp(grid, i, /*expected*/ false);
    }
    return true;
}

bool read_send_data_neighbours(grid_t *grid)
{
    sensor_reading_t data = {0};
    char packed_data[DATA_PACK_SIZE] = {0};
    char recv_packed_data[NB_NEIGHBOURS][DATA_PACK_SIZE] = {{0}};
    sensor_reading_t recv_data[NB_NEIGHBOURS] = {{0}};

    read_data(grid, &data);
    if (data.magnitude <= 2.5f) // todo put in another function ?
        return true;
    if (!pack_data(grid, &data, packed_data))
        return false;
    if (!send_recv_neighbours(grid->comm, grid->neighbours, packed_data, DATA_PACK_SIZE, recv_packed_data))
        return false;
    for (unsigned int i = 0; i < NB_NEIGHBOURS; ++i) {
        if (grid->neighbours[i] == MPI_PROC_NULL)
            continue;
        unpack_data(grid, recv_packed_data[i], &(recv_data[i]));
    };
    // todo cmp
    return true;
}
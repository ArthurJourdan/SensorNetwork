/**
 * @file mpi_utils.c
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "sensor_network.h"
#include "mpi_utils.h"

/**
 * @brief
 * @param comm
 * @param process
 * @param coordinates array of size NB_DIMENSIONS
 * @return
 */
bool get_comm_info(MPI_Comm comm, mpi_info_t *process, int coordinates[NB_DIMENSIONS])
{
    int comm_is_not_world = false;

    if (MPI_Comm_size(comm, &(process->nb_processes)) != MPI_SUCCESS)
        return false;
    if (MPI_Comm_rank(comm, &(process->rank)) != MPI_SUCCESS)
        return false;
    MPI_Comm_compare(MPI_COMM_WORLD, comm, &comm_is_not_world);
    if (comm_is_not_world == MPI_UNEQUAL && coordinates)
        if (MPI_Cart_coords(comm, process->rank, NB_DIMENSIONS, coordinates) != MPI_SUCCESS)
            return false;
    return true;
}

/**
 * @brief
 * @param comm
 * @param neighbours Array of size NB_NEIGHBOURS
 * @return
 */
bool get_neighbours(MPI_Comm comm, int neighbours[NB_NEIGHBOURS])
{
    const int LOW = 0;
    const int HIGH = 1;

    for (int direction = 0; direction < NB_DIMENSIONS; ++direction) {
        neighbours[(direction * 2) + LOW] = MPI_PROC_NULL;
        neighbours[(direction * 2) + HIGH] = MPI_PROC_NULL;
        MPI_Cart_shift(comm, direction, 1, &(neighbours[(direction * 2) + LOW]), &(neighbours[(direction * 2) + HIGH]));
    }
#ifdef DEBUG
    bool print = false;
    for (int i = 0; i < NB_NEIGHBOURS; i++) {
        if (neighbours[i] != MPI_PROC_NULL) {
            printf("%s%i", (i && print ? ", " : ""), neighbours[i]);
            print = true;
        }
    }
    printf("\n");
#endif
    return true;
}

void print_MPI_error(MPI_Status *status)
{
    char buff[1000] = {0};
    int buff_size = 1000;

    MPI_Error_string(status->MPI_ERROR, buff, &buff_size);
    buff[buff_size++] = '\n';
    write(2, buff, buff_size);
}

/*
bool async_send_recv(char send_buff[DATA_PACK_SIZE], char recv_buff[DATA_PACK_SIZE], const int count, const int rank,
    MPI_Comm comm, MPI_Request *send_req, MPI_Request *recv_req)
{
    MPI_Isend(send_buff, count, MPI_PACKED, rank, 0, comm, send_req);
    MPI_Irecv(recv_buff, count, MPI_PACKED, rank, 0, comm, recv_req);
}
*/

/**
 * @brief
 * @param comm
 * @param neighbours Array of size NB_NEIGHBOURS
 * @param buf
 * @param count
 * @param recv_buf needs to be pre-allocated
 * @return
 */
bool send_recv_neighbours(MPI_Comm comm, const int neighbours[NB_NEIGHBOURS], char buf[DATA_PACK_SIZE], int count,
    char recv_buf[NB_NEIGHBOURS][DATA_PACK_SIZE])
{
    bool retval = true;
    MPI_Request send_request[NB_NEIGHBOURS];
    MPI_Request recv_request[NB_NEIGHBOURS];
    MPI_Status status[NB_NEIGHBOURS] = {MPI_SUCCESS};

    for (int i = 0; i < NB_NEIGHBOURS; i++) {
        if (neighbours[i] == MPI_PROC_NULL)
            continue;
        MPI_Isend(buf, count, MPI_PACKED, neighbours[i], 0, comm, &send_request[i]);
        MPI_Irecv(recv_buf[i], count, MPI_PACKED, neighbours[i], 0, comm, &recv_request[i]);
        // todo update with new version
    }

    for (unsigned short i = 0; i < NB_NEIGHBOURS; i++) {
        if (neighbours[i] == MPI_PROC_NULL)
            continue;
        MPI_Wait(&send_request[i], &status[i]);
        if (status[i].MPI_ERROR != MPI_SUCCESS) {
            retval = false;
            print_MPI_error(&status[i]);
        }
        MPI_Wait(&recv_request[i], &status[i]);
        if (status[i].MPI_ERROR != MPI_SUCCESS) {
            retval = false;
            print_MPI_error(&status[i]);
        }
    }
    return retval;
}

void print_coordinates(const int coordinates[NB_DIMENSIONS])
{
    printf("[");
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        printf("%i", coordinates[i]);
        if (i < NB_DIMENSIONS - 1)
            printf(", ");
        else
            printf("]\n");
    }
}
void print_float_coordinates(const float coordinates[NB_DIMENSIONS])
{
    printf("[");
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        printf("%f", coordinates[i]);
        if (i < NB_DIMENSIONS - 1)
            printf(", ");
        else
            printf("]\n");
    }
}
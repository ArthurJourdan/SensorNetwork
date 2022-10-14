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

#include "stdio.h" // todo remove

bool create_sensor_data_type(MPI_Datatype *new_data_type)
{
    const int array_of_blocklength[4] = {9, NB_DIMENSIONS, 1, 1};
    const MPI_Aint array_of_displacement[4] = {0};
    MPI_Datatype array_of_types[4] = {MPI_INT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT};

    MPI_Type_create_struct(
        DATA_PACK_ELEM_NB, array_of_blocklength, array_of_displacement, array_of_types, new_data_type);
    return true;
}

bool read_send_data_neighbours(grid_t *grid)
{
    //    sensor_reading_t sensor_data;
    char packed_data[DATA_PACK_SIZE] = {0};
    char recv_packed_data[NB_NEIGHBOURS][DATA_PACK_SIZE] = {{0}};
    sensor_reading_t recv_data[NB_NEIGHBOURS] = {{0}};

    if (!read_pack_data(grid, packed_data))
        return false;

    int comm_is_not_world = false;
    MPI_Comm_compare(MPI_COMM_WORLD, grid->comm, &comm_is_not_world);
    if (comm_is_not_world != MPI_UNEQUAL) {
        printf("\n\n\n%s.\n", "not");
        return false;
    }

    if (!send_recv_neighbours(grid->comm, grid->neighbours, packed_data, DATA_PACK_SIZE, recv_packed_data))
        return false;
    for (unsigned int i = 0; i < NB_NEIGHBOURS; ++i) {
        if (grid->neighbours[i] == MPI_PROC_NULL)
            continue;
        //        write(1, recv_packed_data[i], DATA_PACK_SIZE);
        //        printf(".\n");
        unpack_data(grid, recv_packed_data[i], &(recv_data[i]));
        print_data(&(recv_data[i]));
    };
    return true;
}
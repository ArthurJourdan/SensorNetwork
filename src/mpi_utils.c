/**
 * @file mpi_utils.c
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>

#include "grid.h"
#include "mpi_utils.h"

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

static bool get_neighbors(MPI_Comm comm, int neighbors[NB_DIMENSIONS * 2])
{
    const int LOW = 0;
    const int HIGH = 1;

    for (int direction = 0; direction < NB_DIMENSIONS; ++direction) {
        neighbors[(direction * 2) + LOW] = MPI_PROC_NULL;
        neighbors[(direction * 2) + HIGH] = MPI_PROC_NULL;
        MPI_Cart_shift(comm, direction, 1, &(neighbors[(direction * 2) + LOW]), &(neighbors[(direction * 2) + HIGH]));
    }
    //    printf("neighbors of %i : ", process->rank);
    for (int i = 0; i < NB_DIMENSIONS * 2; i++) {
        if (neighbors[i] != MPI_PROC_NULL) {
            //            printf("%i%s", neighbors[i], (i == (NB_DIMENSIONS * 2) - 1 ? "" : ", "));
        }
    }
    //    printf("\n");
    return true;
}

// bool send_to_neighbors(MPI_Comm comm, mpi_info_t *process, grid_t *grid, const int neighbors[NB_DIMENSIONS * 2])
//{
//     //    int random_prime = get_random_prime();
//     // artifact from lab from which this code is taken
//
//     MPI_Request send_request[NB_DIMENSIONS * 2 /* each dimension with possibly 2 neighbors each*/];
//     MPI_Request recv_request[NB_DIMENSIONS * 2 /* each dimension with possibly 2 neighbors each*/];
//     MPI_Status status[NB_DIMENSIONS * 2 /* each dimension with possibly 2 neighbors each*/];
//     int received_primes[NB_DIMENSIONS * 2 /* each dimension with possibly 2 neighbors each*/] = {-1};
//
//     for (int i = 0; i < NB_DIMENSIONS * 2; i++) {
//         if (neighbors[i] != MPI_PROC_NULL) {
//             //            MPI_Isend(&random_prime, 1, MPI_INT, neighbors[i], 0, comm, &(send_request[i]));
//             //            MPI_Irecv(&(received_primes[i]), 1, MPI_INT, neighbors[i], 0, comm, &(recv_request[i]));
//         }
//     }
//     for (unsigned short i = 0; i < NB_DIMENSIONS * 2; i++) {
//         if (neighbors[i] != MPI_PROC_NULL) {
//             //            MPI_Wait(&(send_request[i]), &(status[i]));
//             //            MPI_Wait(&(recv_request[i]), &(status[i]));
//             //            printf("Process %i\treceived prime %i\tfrom %i\n", process->rank, received_primes[i],
//             //            neighbors[i]);
//         }
//     }
//     //    output_common_primes(random_prime, received_primes, grid->process_position);
//     // artifact from lab from which this code is taken
//     return true;
// }

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
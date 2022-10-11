/**
 * @file main.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include "base_station.h"
#include <stdio.h>
#include <unistd.h>
#include <mpi/mpi.h>
#include <stdlib.h>

#include "grid.h"
#include "mpi_utils.h"
#include "utils.h"

void init_topology(mpi_info_t *process, grid_t *grid); // todo put in header

static bool init_grid_topology(mpi_info_t *process, grid_t *grid)
{
    int total_grid_elements = 1;

    if (process->rank == 0) {
        get_grid_size(grid); // get grid size from user input
    }
    MPI_Bcast(grid->size, NB_DIMENSIONS, MPI_UNSIGNED, 0, MPI_COMM_WORLD); // send grid size to every process
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        total_grid_elements *= grid->size[i];
    }
    if (total_grid_elements != process->nb_processes - 1) {
        if (process->rank == 0) {
            dprintf(STDERR_FILENO,
                "Input grid size (%u) is different from number of MPI processes (%i withouth the master)\n",
                total_grid_elements,
                process->nb_processes - 1);
        }
        return false;
    }
    return init_grid(MPI_COMM_WORLD, &grid->communicator, process, grid);
}

static bool init(int ac, char *av[], mpi_info_t *process, grid_t *grid)
{
    MPI_Init(&ac, &av);
    get_comm_info(MPI_COMM_WORLD, process, NULL);

    return init_grid_topology(process, grid);
}

static void __attribute__((constructor)) init_rand(void)
{
    srand(getpid() + time(NULL));
}

int main(int ac, char *av[])
{
    grid_t my_grid = {0};
    mpi_info_t process = {0, 0};
    bool ret_val = true;

    ret_val = init(ac, av, &process, &my_grid);
    if (process.rank == 0) {
        base_station(process);
    }
    print_coordinates(my_grid.process_position);
    if (!ret_val)
        return 1;
    return 0;
}

static void __attribute__((destructor)) finalize_MPI(void)
{
    MPI_Finalize();
}
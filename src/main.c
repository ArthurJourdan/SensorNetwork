/**
 * @file main.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include "base_station.h"
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>

#include "grid.h"
#include "mpi_utils.h"
#include "sensor.h"
#include "utils.h"

static bool init(int ac, char **av, mpi_info_t *process, grid_t *grid)
{
    MPI_Init(&ac, &av);
    get_comm_info(MPI_COMM_WORLD, process, NULL);

    return init_grid(process, grid);
}

static void __attribute__((constructor)) init_rand(void)
{
    srand(getpid() + time(NULL));
}

int main(int ac, char **av)
{
    grid_t my_grid = {0};
    mpi_info_t process = {0, 0};
    bool ret_val = true;
    int comm_is_not_world = false;

    ret_val = init(ac, av, &process, &my_grid);
    MPI_Comm_compare(MPI_COMM_WORLD, my_grid.comm, &comm_is_not_world);
    if (comm_is_not_world == MPI_UNEQUAL) {
        launch_sensor(&my_grid);
    } //    else {
    //        base_station(process);
    //    }
    if (!ret_val)
        return 1;
    return 0;
}

static void __attribute__((destructor)) finalize_MPI(void)
{
    MPI_Finalize();
}
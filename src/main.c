/**
 * @file main.h
 * @author Arthur Jourdan
 * @date of creation 9/09/22.
 * @brief TODO
 */

#include "grid.h"
#include "mpi_utils.h"
#include "utils.h"

bool init(int ac, char *av[], mpi_info_t *process, grid_t *grid)
{
    bool ret_val = get_grid_size(&grid->size);

    if (!ret_val)
        return false;

    init_MPI(&ac, &av, process);
    init_rand(process->rank);
    return true;
}

// todo attribute constructor of program (for metrics)
//  start chrono and init global struct
int main(int ac, char *av[])
{
    grid_t my_grid = {0};
    mpi_info_t process = {0, 0};
    bool ret_val = true;

    ret_val = init(ac, av, &process, &my_grid);
    if (!ret_val)
        return 1;
    return 0;
}
// todo attribute destructor of program (for metrics)
//  stop chrono and write global struct to readable file
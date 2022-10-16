/**
 * @file fault_detection_thread.c
 * @author Arthur Jourdan
 * @date of creation 16/10/22.
 * @brief TODO
 */

#include "base_station.h"

#include <mpi.h>
#include <stdlib.h>
#include <string.h>

bool check_every_sensor(const int nb_sensors, int *dead_sensors_count)
{
    bool is_alive = true;
    int flag = 0;
    MPI_Request *requests = malloc(sizeof(MPI_Request) * nb_sensors);
    memset(requests, 0, sizeof(MPI_Request) * nb_sensors);
    MPI_Status *status = malloc(sizeof(MPI_Status) * nb_sensors);
    memset(status, 0, sizeof(MPI_Status) * nb_sensors);
    char **buff = malloc(sizeof(char *) * nb_sensors);

    for (int i = 1; i < nb_sensors; ++i) {
        buff[i] = malloc(sizeof(char) * strlen(STR_ALIVE));
        MPI_Irecv(buff, strlen(STR_ALIVE), MPI_CHAR, i, 0, MPI_COMM_WORLD, &requests[i]);
    }

    for (int i = 1; i < nb_sensors; ++i) {
        flag = 0;
        is_alive = true;
        if (MPI_Request_get_status(requests[i], &flag, &status[i]) != MPI_SUCCESS)
            is_alive = false;
        if (!flag)
            is_alive = false;
        if (status[i].MPI_ERROR != MPI_SUCCESS)
            is_alive = false;
        free(buff[i]);
        MPI_Cancel(&requests[i]);
        if (!is_alive) {
            dead_sensors_count[i]++;
            printf("%i is not alive.\n", i);
        } else {
            dead_sensors_count[i] = 0;
            printf("%i is alive.\n", i);
        }
    }
    free(requests);
    free(buff);
    return true;
}

void fault_detection_thread(void *param)
{
    mpi_info_t *process = (mpi_info_t *) param;
    int *dead_sensors_count = malloc(sizeof(int) * process->nb_processes);
    memset(dead_sensors_count, 0, sizeof(int) * process->nb_processes);
    pthread_mutex_lock(&MUTEX_EXIT);

    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&MUTEX_EXIT);
        check_every_sensor(process->nb_processes, dead_sensors_count);
        for (int i = 0; i < process->nb_processes; ++i) {
            if (dead_sensors_count[i] > 2) {
                pthread_mutex_lock(&MUTEX_EXIT);
                THREADS_EXIT = true;
                pthread_mutex_unlock(&MUTEX_EXIT);
            }
        }
        pthread_mutex_lock(&MUTEX_EXIT);
    }
    pthread_mutex_unlock(&MUTEX_EXIT);
}
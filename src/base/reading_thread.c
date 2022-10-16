#include <unistd.h>
#include <mpi.h>
#include <pthread.h>
#include "base_station.h"

void reading_thread(void *ptr)
{
    int size = *(int *) ptr;
    MPI_Status status;
    int recv, msgAvail;

    pthread_mutex_lock(&MUTEX_EXIT);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&MUTEX_EXIT);
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &msgAvail, &status);

        sleep(POLLING_RATE);

        if (msgAvail) {
            MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        }
        /* the base station compares the received report with the
        data in the shared global array (which is populated by the balloon seismic sensor) */

        // If there is a match, the base station logs the report as a conclusive event (conclusive alert).
        // if (match) {}

        // If there is no match, the base station logs the report as an inconclusive event (inconclusive alert)
        // else {}
        pthread_mutex_lock(&MUTEX_EXIT);
    }
    pthread_mutex_unlock(&MUTEX_EXIT);
    /* Continuing from (f), the base station sends a termination message to the sensor nodes to properly
    shutdown. The base station also sends a termination signal to the balloon seismic sensor to properly
    shutdown. */
}

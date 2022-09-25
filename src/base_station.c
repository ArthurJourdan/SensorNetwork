#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <mpi.h>
#include "base_station.h"
#include "sensor.h"
#include "balloon.h"

#define POLLING_RATE 0.5 // seconds

int THREADS_EXIT = 0;

void reading_thread(void *ptr)
{
    int size = *(int *) ptr;
    MPI_Status status;
    int recv, msgAvail;

    while (!THREADS_EXIT) {
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
    }

    /* Continuing from (f), the base station sends a termination message to the sensor nodes to properly
    shutdown. The base station also sends a termination signal to the balloon seismic sensor to properly
    shutdown. */
}

void base_station(mpi_info_t process)
{
    // main thread
    // reads and waits for sentinel value
    char sentinel, keyPressed;
    pthread_attr_t attr;
    pthread_t reading_tid, balloon_tid;
    char userInput[64];

    printf("Sentinel value: ");
    scanf(" %c", &sentinel);

    pthread_attr_init(&attr);
    pthread_create(&reading_tid, &attr, (void *) reading_thread, &process);
    pthread_create(&balloon_tid, &attr, (void *) balloon_thread, &process);

    while (!THREADS_EXIT) {
        scanf("%s", userInput); // Check new user input
        if (strlen(userInput) == 1 && userInput[0] == sentinel)
            THREADS_EXIT = 1;
    }

    pthread_join(balloon_tid, NULL);
    pthread_join(reading_tid, NULL);
}
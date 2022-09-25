#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>
#include "base_station.h"

void balloon_thread(void *ptr)
{
    while (true) {
        /* This thread periodically produces seismic readings. This reading consists of date, time, latitude and
        longitude of the earthquake point, magnitude, and depth of the quake from the sensor. However, the
        generated magnitude always exceeds the predefined threshold (e.g., magnitude > 2.5) */
    }
}

void reading_thread(void *ptr)
{
    int size = *(int *) ptr;
    MPI_Status status;
    int recv;

    while (true) {
        MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        /* the base station compares the received report with the
        data in the shared global array (which is populated by the balloon seismic sensor) */

        // If there is a match, the base station logs the report as a conclusive event (conclusive alert).
        // if (match) {}

        // If there is no match, the base station logs the report as an inconclusive event (inconclusive alert)
        // else {}
    }
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

    while (true) {
      scanf("%s", userInput);

      if (strlen(userInput) == 1 && userInput[0] == sentinel) {
        break;
      }
    }

    // TODO change to global cancel variable 
    pthread_cancel(reading_tid);
    pthread_cancel(balloon_tid);
}
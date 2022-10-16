#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <mpi.h>
#include <signal.h>
#include "base_station.h"
#include "sensor.h"
#include "balloon.h"

#define POLLING_RATE 0.5 // seconds

int THREADS_EXIT = 0;
pthread_mutex_t MUTEX_EXIT;

void sig_func(int sig)
{
#ifdef DEBUG
    printf("Signal Handler has been called\nEXITING...\n");
#endif

    pthread_mutex_lock(&MUTEX_EXIT);
    THREADS_EXIT = 1;
    pthread_mutex_unlock(&MUTEX_EXIT);
}

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

void base_station(mpi_info_t process)
{
    // main thread
    // reads and waits for sentinel value
    char sentinel, keyPressed;
    pthread_attr_t attr;
    pthread_t reading_tid, balloon_tid;
    char userInput[64];

    printf("Sentinel value: ");
    fflush(stdout);
    scanf(" %c", &sentinel);

    signal(SIGQUIT, sig_func); // Register signal handler before going multithread

    pthread_attr_init(&attr);
    pthread_create(&reading_tid, &attr, (void *) reading_thread, &process);
    pthread_create(&balloon_tid, &attr, (void *) balloon_thread, &process);

    pthread_mutex_init(&MUTEX_EXIT, NULL);
    pthread_mutex_lock(&MUTEX_EXIT);

    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&MUTEX_EXIT);
        scanf("%s", userInput); // Check new user input
        if (strlen(userInput) == 1 && userInput[0] == sentinel)
            THREADS_EXIT = 1;
        pthread_mutex_lock(&MUTEX_EXIT);
    }

    pthread_mutex_unlock(&MUTEX_EXIT);

    // Send quit messages to the sensor network
    for (int dest = 1; dest < process.nb_processes; dest++) {
        MPI_Send(STR_EXIT, 7, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
    }

    // Wait for threads to exit
    pthread_join(balloon_tid, NULL);
    pthread_join(reading_tid, NULL);
}
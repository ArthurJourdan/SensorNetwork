#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <mpi.h>
#include <signal.h>
#include "base_station.h"
#include "sensor.h"

int THREADS_EXIT = 0;
pthread_mutex_t MUTEX_EXIT;

static pthread_attr_t attr;
static pthread_t reading_tid, balloon_tid;

void sig_func(int sig)
{
#ifdef DEBUG
    printf("Signal Handler has been called\nEXITING...\n");
#endif

    pthread_mutex_lock(&MUTEX_EXIT);
    THREADS_EXIT = 1;
    pthread_mutex_unlock(&MUTEX_EXIT);
}

void sentinel_detected()
{
    pthread_mutex_lock(&MUTEX_EXIT);
    THREADS_EXIT = 1;
    pthread_mutex_unlock(&MUTEX_EXIT);
#ifdef DEBUG
    printf("Sentinel value was detected.\nCommencing shutdown procedure...\n");
#endif
}

void base_init_pthread(void *params)
{
    pthread_attr_init(&attr);
    pthread_create(&reading_tid, &attr, (void *) reading_thread, params);
    pthread_create(&balloon_tid, &attr, (void *) balloon_thread, params);

    pthread_mutex_init(&MUTEX_EXIT, NULL);
}

void base_quit(mpi_info_t process)
{
    // Wait for threads to exit
    pthread_join(balloon_tid, NULL);
    pthread_join(reading_tid, NULL);

#ifdef DEBUG
    printf("Base station has exited\n");
#endif
}

void base_station(mpi_info_t process)
{
    // main thread
    // reads and waits for sentinel value
    char sentinel;
    char userInput[64];

    printf("Sentinel value: ");
    fflush(stdout);
    scanf(" %c", &sentinel);

#ifdef DEBUG
    printf("Sentinel value is: %c\n", sentinel);
#endif

    signal(SIGQUIT, sig_func); // Register signal handler before going multithread

    base_init_pthread(&process);

    pthread_mutex_lock(&MUTEX_EXIT);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&MUTEX_EXIT);
        scanf("%s", userInput); // Check new user input
        if (strlen(userInput) == 1 && userInput[0] == sentinel)
            sentinel_detected();
        pthread_mutex_lock(&MUTEX_EXIT);
    }
    pthread_mutex_unlock(&MUTEX_EXIT);
    base_quit(process);
}
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <mpi.h>
#include "base_station.h"
#include "sensor.h"

int THREADS_EXIT = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static pthread_attr_t attr;
static pthread_t reading_tid, balloon_tid, fault_detection_tid;

void sig_func(int sig)
{
    if (sig == SIGQUIT) {
#ifdef DEBUG
        printf("Signal Handler has been called\nEXITING...\n");
#endif
        pthread_mutex_lock(&lock);
        THREADS_EXIT = 1;
        pthread_mutex_unlock(&lock);
    }
}

static void sentinel_detected()
{
    pthread_mutex_lock(&lock);
    THREADS_EXIT = 1;
    pthread_mutex_unlock(&lock);
#ifdef DEBUG
    printf("Sentinel value was detected.\nCommencing shutdown procedure...\n");
#endif
}

static void init_pthread(void *params)
{
    pthread_attr_init(&attr);
    pthread_create(&balloon_tid, &attr, (void *) balloon_thread, params);
    pthread_create(&reading_tid, &attr, (void *) reading_thread, params);
    pthread_create(&fault_detection_tid, &attr, (void *) fault_detection_thread, params); // todo
}

static char ask_sentinel_val()
{
    char sentinel = 0;
    printf("Sentinel value: ");
    fflush(stdout);
    scanf(" %c", &sentinel);
#ifdef DEBUG
    printf("Sentinel value is: %c\n", sentinel);
#endif
    return sentinel;
}

static void on_quit()
{
    // Wait for threads to exit
    pthread_join(balloon_tid, NULL);
    pthread_join(reading_tid, NULL);

    pthread_mutex_destroy(&lock);

#ifdef DEBUG
    printf("Base station has exited\n");
#endif
}

/**
 * @brief Entry point for base_station process
 * @param process
 */
void base_station(mpi_info_t process)
{
    char sentinel;
    char userInput[64] = {0};

    sentinel = ask_sentinel_val();
    signal(SIGQUIT, sig_func); // Register signal handler before going multithread
    init_pthread(&process);

    // Main loop: watches stdin for sentinel value
    pthread_mutex_lock(&lock);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&lock);
        scanf("%s", userInput); // Check new user input
        if (strlen(userInput) == 1 && userInput[0] == sentinel)
            sentinel_detected();
        pthread_mutex_lock(&lock);
    }
    pthread_mutex_unlock(&lock);

    on_quit();
}
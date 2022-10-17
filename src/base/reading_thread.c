/**
 * Reading Thread is responsible for communicating between
 * the sensor network and base station, and acts like a bridge
 * between the two.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <mpi.h>
#include <pthread.h>
#include "base_station.h"
#include "sensor.h"
#include "sensor_network.h"
#include "mpi_utils.h"
#include "metrics.h"

#define MAGNITUDE_THRESHOLD 5
#define DEPTH_THRESHOLD     500

static int fd;
static MPI_Status status;
static char recv_bufs[2][DATA_PACK_SIZE];
static sensor_reading_t recv_data[2];

static void on_quit(mpi_info_t *process)
{
    // Send quit messages to the sensor network
    /*MPI_Request request;*/
    //     Send quit messages to the sensor network
    for (int dest = 1; dest < process->nb_processes; dest++) {
        MPI_Send(STR_EXIT, 7, MPI_CHAR, dest, 0, MPI_COMM_WORLD /*, &request*/);
    }

#ifdef DEBUG
    printf("Reading thread has exited\n");
#endif
}

static void on_msg_avail()
{
    // Receive data
    MPI_Recv(recv_bufs[0], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[0], &recv_data[0]);

    // Receive neighbour
    MPI_Recv(recv_bufs[1], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[1], &recv_data[1]);

    sensor_reading_t balloon_data = balloon_buffer[balloon_index];
    /* the base station compares the received report with the
    data in the shared global array (which is populated by the balloon seismic sensor) */
    // int match_magnitude = recv_data[0].magnitude < (balloon_data.magnitude + MAGNITUDE_THRESHOLD)
    //     && recv_data[0].magnitude > (balloon_data.magnitude - MAGNITUDE_THRESHOLD);

    // int match_depth = recv_data[0].depth < (balloon_data.depth + DEPTH_THRESHOLD)
    //     && recv_data[0].depth > (balloon_data.depth - DEPTH_THRESHOLD);

    bool match = sensor_data_cmp(&recv_data[0], &balloon_data);

    // #ifdef DEBUG
    printf("\n\n\n");

    printf("Balloon data: \n");
    pthread_mutex_lock(&lock);
    print_data(&balloon_buffer[balloon_index]);
    pthread_mutex_unlock(&lock);

    printf("Received data from process: %d\n", status.MPI_SOURCE);
    print_data(&recv_data[0]);

    printf("Received neighbour data from process: %d\n", status.MPI_SOURCE);
    print_data(&recv_data[1]);

    if (match)
        printf("Match\n");
    else
        printf("No Match\n");

    printf("\n\n\n");
    // #endif

    // If there is a match, the base station logs the report as a conclusive event (conclusive alert).
    // If there is no match, the base station logs the report as an inconclusive event (inconclusive alert)
    // else {}
    // char *event_type;
    // if (match)
    //     stpcpy(event_type, "Conclusive");
    // else
    //     stpcpy(event_type, "Inconclusive");

    sensor_reading_t data_array[3];
    data_array[0] = recv_data[0];
    data_array[1] = recv_data[1];
    data_array[2] = balloon_data;

    dprint_data_array(fd, data_array, 3);

    // fprintf(file,
    //     "%s %f %f %f %f %s\n",
    //     asctime(recv_data[0].timestamp),
    //     recv_data[0].coordinates[0],
    //     recv_data[0].coordinates[1],
    //     recv_data[0].magnitude,
    //     recv_data[0].depth,
    //     event_type);
}

/**
 * @brief Entry point for reading thread
 * @param ptr
 */
void reading_thread(void *ptr)
{
    int msgAvail;
    mpi_info_t process = *(mpi_info_t *) ptr;
    memset(recv_bufs, 0, sizeof(char) * (DATA_PACK_SIZE));
    fd = open("alert.txt", O_CREAT | O_WRONLY);

    pthread_mutex_lock(&lock);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&lock);
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &msgAvail, &status);

#ifdef DEBUG
        printf("Balloon data: \n");
        print_data(&balloon_buffer[balloon_index]);
#endif
        if (msgAvail)
            on_msg_avail();

        sleep(POLLING_RATE);
        pthread_mutex_lock(&lock);
    }
    pthread_mutex_unlock(&lock);
    /* Continuing from (f), the base station sends a termination message to the sensor nodes to properly
    shutdown. The base station also sends a termination signal to the balloon seismic sensor to properly
    shutdown. */
    on_quit(&process);
}
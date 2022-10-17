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

static int fd_conclusive, fd_inconclusive;
static MPI_Status status;
static char recv_bufs[2][DATA_PACK_SIZE];
sensor_reading_t recv_data[2];
base_station_metrics_t metrics;

static void on_init(mpi_info_t *process)
{
    memset(recv_bufs, 0, sizeof(char) * DATA_PACK_SIZE * 2);
    fd_conclusive = open("alert_conclusive.txt", O_CREAT | O_WRONLY, 0666);
    fd_inconclusive = open("alert_inconclusive.txt", O_CREAT | O_WRONLY, 0666);
    init_metrics(&metrics, process->nb_processes - 1);
}

static void on_quit(mpi_info_t *process)
{
    const int fd_metrics = open("metrics.txt", O_CREAT | O_WRONLY, 0666);
    if (fd_metrics != -1) {
        dprint_metrics(fd_metrics, &metrics);
        close(fd_metrics);
    }

    // Send quit messages to the sensor network
    for (int dest = 1; dest < process->nb_processes; dest++) {
        printf("Sending exit message to process %d\n", dest);
        MPI_Send(STR_EXIT, (int) sizeof(STR_EXIT) * sizeof(char) - 1, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
    }

    free_metrics(&metrics);
    close(fd_conclusive);
    close(fd_inconclusive);
#ifdef DEBUG
    printf("Reading thread has exited\n");
#endif
}

static void on_msg_avail()
{
    int sources[2];
    sensor_reading_t balloon_data;

    // Receive data
    MPI_Recv(recv_bufs[0], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[0], &recv_data[0]);
    sources[0] = status.MPI_SOURCE;

    // Receive neighbour
    MPI_Recv(recv_bufs[1], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[1], &recv_data[1]);
    sources[1] = status.MPI_SOURCE;

    /* the base station compares the received report with the
    data in the shared global array (which is populated by the balloon seismic sensor) */
    pthread_mutex_lock(&lock);
    copy_sensor_data(&balloon_buffer[balloon_index], &balloon_data);
    pthread_mutex_unlock(&lock);
    bool match = sensor_data_cmp(&recv_data[0], &balloon_data);

    // Log Metrics
    metrics.sensors_metrics[sources[0] - 1].nb_messages++;
    metrics.sensors_metrics[sources[1] - 1].nb_messages++;

    metrics.total_nb_messages++;

#ifdef DEBUG
    printf("\n\n\n");

    printf("Balloon data: \n");
    pthread_mutex_lock(&lock);
    print_data(&balloon_data);
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
#endif

    // If there is a match, the base station logs the report as a conclusive event (conclusive alert).
    // If there is no match, the base station logs the report as an inconclusive event (inconclusive alert)
    sensor_reading_t data_array[3];
    data_array[0] = recv_data[0];
    data_array[1] = recv_data[1];
    data_array[2] = balloon_data;

    if (match) {
        metrics.sensors_metrics[sources[0] - 1].nb_alerts++;
        metrics.sensors_metrics[sources[1] - 1].nb_alerts++;
        metrics.total_nb_alerts++;
        dprint_data_array(fd_conclusive, data_array, 3);
    } else
        dprint_data_array(fd_inconclusive, data_array, 3);
}

/**
 * @brief Entry point for reading thread
 * @param ptr
 */
void reading_thread(void *ptr)
{
    int msgAvail;
    mpi_info_t process = *(mpi_info_t *) ptr;

    on_init(&process);

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
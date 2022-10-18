/**
 * Reading Thread is responsible for communicating between
 * the sensor network and base station, and acts like a bridge
 * between the two.
 */

#include "base_station.h"
#include "sensor.h"
#include "sensor_network.h"
#include "mpi_utils.h"
#include "metrics.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <mpi.h>
#include <pthread.h>

static int fd_conclusive, fd_inconclusive;
base_station_metrics_t metrics;

static void on_init(mpi_info_t *process)
{
    fd_conclusive = open("alert_conclusive.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    dprint_data_array(fd_conclusive, NULL, 0);
    fd_inconclusive = open("alert_inconclusive.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    dprint_data_array(fd_inconclusive, NULL, 0);
    init_metrics(&metrics, process->nb_processes - 1);
}

static void on_quit(mpi_info_t *process)
{
    // Send quit messages to the sensor network
    for (int dest = 1; dest < process->nb_processes; dest++) {
        printf("Sending exit message to process %d\n", dest);
        MPI_Send(STR_EXIT,
            (int) sizeof(STR_EXIT) * sizeof(char) - 1,
            MPI_CHAR,
            dest,
            TAG_BASE_TO_SENSOR_EXIT,
            MPI_COMM_WORLD);
    }

    close(fd_conclusive);
    close(fd_inconclusive);
#ifdef DEBUG
    printf("Reading thread has exited\n");
#endif
    const int fd_metrics = open("metrics.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fd_metrics != -1) {
        dprint_metrics(fd_metrics, &metrics);
        close(fd_metrics);
    }
    free_metrics(&metrics);
}

static void on_msg_avail()
{
    MPI_Status status;
    int sources[2] = {0};
    sensor_reading_t recv_data[2] = {0};
    sensor_reading_t balloon_data = {0};

    static char recv_bufs[2][DATA_PACK_SIZE];
    memset(recv_bufs[0], 0, sizeof(char) * DATA_PACK_SIZE);
    memset(recv_bufs[1], 0, sizeof(char) * DATA_PACK_SIZE);

    // Receive data
    MPI_Recv(recv_bufs[0], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, TAG_SENSOR_TO_BASE, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[0], &recv_data[0]);
    sources[0] = status.MPI_SOURCE;

    // Receive neighbour
    MPI_Recv(recv_bufs[1], DATA_PACK_SIZE, MPI_PACKED, MPI_ANY_SOURCE, TAG_SENSOR_TO_BASE, MPI_COMM_WORLD, &status);
    unpack_data(MPI_COMM_WORLD, recv_bufs[1], &recv_data[1]);
    sources[1] = status.MPI_SOURCE;

    /* the base station compares the received report with the
    data in the shared global array (which is populated by the balloon seismic sensor) */
    pthread_mutex_lock(&lock);
    copy_sensor_data(&balloon_buffer, &balloon_data);
    pthread_mutex_unlock(&lock);
    bool match[2] = {false};

    match[0] = sensor_data_cmp(&recv_data[0], &balloon_data);
    match[1] = sensor_data_cmp(&recv_data[1], &balloon_data);

    // Log Metrics
    metrics.sensors_metrics[sources[0]].nb_messages += 1;
    metrics.sensors_metrics[sources[1]].nb_messages += 1;

    metrics.total_nb_messages += 2;

#ifdef DEBUG
    printf("\n\n\n");

    printf("Balloon data: \n");
    pthread_mutex_lock(&lock);
    print_data(&balloon_data);
    pthread_mutex_unlock(&lock);

    printf("Received data from process: %d\n", status.MPI_SOURCE);
    print_data(&recv_data[0]);

    if (match[0])
        printf("Match\n");
    else
        printf("No Match\n");

    printf("Received neighbour data from process: %d\n", status.MPI_SOURCE);
    print_data(&recv_data[1]);

    if (match[1])
        printf("Match\n");
    else
        printf("No Match\n");

    printf("\n\n\n");
#endif

    // If there is a match, the base station logs the report as a conclusive event (conclusive alert).
    // If there is no match, the base station logs the report as an inconclusive event (inconclusive alert)

    if (match[0] || match[1]) {
        if (match[0]) {
            metrics.sensors_metrics[sources[0]].nb_alerts += 1;
            metrics.total_nb_alerts += 1;
            dprintf(fd_conclusive, "sender,\t\t");
            dprint_data_one_liner(fd_conclusive, &recv_data[0]);
        }
        if (match[1]) {
            metrics.sensors_metrics[sources[1]].nb_alerts += 1;
            metrics.total_nb_alerts += 1;
            dprintf(fd_conclusive, "neighbour,\t");
            dprint_data_one_liner(fd_conclusive, &recv_data[1]);
        }
        dprintf(fd_conclusive, "balloon,\t");
        dprint_data_one_liner(fd_conclusive, &balloon_data);
    } else {
        dprintf(fd_inconclusive, "sender,\t\t");
        dprint_data_one_liner(fd_inconclusive, &recv_data[0]);
        dprintf(fd_inconclusive, "neighbour,\t");
        dprint_data_one_liner(fd_inconclusive, &recv_data[1]);
        dprintf(fd_inconclusive, "balloon,\t");
        dprint_data_one_liner(fd_inconclusive, &balloon_data);
    }
}

/**
 * @brief Entry point for reading thread
 * @param ptr
 */
void reading_thread(void *ptr)
{
    int msgAvail;
    mpi_info_t process = *(mpi_info_t *) ptr;
    MPI_Status status;

    on_init(&process);

    pthread_mutex_lock(&lock);
    while (!THREADS_EXIT) {
        pthread_mutex_unlock(&lock);
        MPI_Iprobe(MPI_ANY_SOURCE, TAG_SENSOR_TO_BASE, MPI_COMM_WORLD, &msgAvail, &status);

#ifdef DEBUG
        printf("Balloon data: \n");
        print_data(&balloon_buffer);
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
/**
 * @file metrics.c
 * @author Arthur Jourdan
 * @date of creation 16/10/22.
 * @brief TODO
 */

#include "metrics.h"

#include <stdlib.h>
#include <stdio.h>

bool init_sensor_metrics(const int rank, sensor_message_metrics_t *sensor_metrics)
{
    if (!sensor_metrics)
        return false;
    sensor_metrics->rank = rank;
    sensor_metrics->nb_messages = 0;
    sensor_metrics->nb_alerts = 0;
    return true;
}

bool init_metrics(base_station_metrics_t *metrics, const unsigned int nb_sensors)
{
    bool retval = true;

    if (!metrics)
        return false;
    metrics->start_time = time(NULL);
    metrics->total_nb_alerts = 0;
    metrics->total_nb_messages = 0;
    metrics->nb_sensors = nb_sensors;
    if (nb_sensors) {
        metrics->sensors_metrics = malloc(sizeof(sensor_message_metrics_t) * nb_sensors);
        if (!metrics->sensors_metrics)
            return false;
        for (unsigned int i = 0; i < nb_sensors; ++i) {
            if (!init_sensor_metrics(i + 1, &metrics->sensors_metrics[i]))
                retval = false;
        }
    }
    return retval;
}

static inline void dprint_sensor_metrics(const int fd, sensor_message_metrics_t *sensor_metrics)
{
    dprintf(fd, "%i, %u, %u\n", sensor_metrics->rank, sensor_metrics->nb_messages, sensor_metrics->nb_alerts);
}

static inline void dprint_sensors_metrics(
    const int fd, sensor_message_metrics_t *sensor_metrics, const unsigned int nb_sensors)
{
    dprintf(fd, "Information per sensor :\n");
    dprintf(fd, "rank, number of messages, number of alerts\n");
    for (unsigned int i = 0; i < nb_sensors; ++i) {
        dprint_sensor_metrics(fd, &sensor_metrics[i]);
    }
}

void dprint_metrics(const int fd, base_station_metrics_t *metrics)
{
    time_t stop_time = time(NULL);

    dprintf(fd, "### Simulation metrics ###\n");
    dprintf(fd, "Start time : %s", ctime(&metrics->start_time));
    dprintf(fd, "Total number of messages : %u\n", metrics->total_nb_messages);
    dprintf(fd, "Total number of alerts : %u\n", metrics->total_nb_alerts);
    dprintf(fd, "Stop time : %s\n", ctime(&stop_time));
    dprint_sensors_metrics(fd, metrics->sensors_metrics, metrics->nb_sensors);
}

bool free_metrics(base_station_metrics_t *metrics)
{
    if (metrics->sensors_metrics) {
        free(metrics->sensors_metrics);
        return true;
    }
    return false;
}
/**
 * @file metrics.h
 * @author Arthur Jourdan
 * @date of creation 15/09/22.
 * @brief TODO
 */

#ifndef MAKEFILE_METRICS_H
#define MAKEFILE_METRICS_H

#include "utils.h"
#include "mpi_utils.h"

typedef struct
{
    int rank;
    unsigned int nb_messages;
    unsigned int nb_alerts;
} sensor_message_metrics_t;

typedef struct
{
    time_t start_time;
    /**
     * @brief Sum of all sensor alerts
     */
    unsigned int total_nb_alerts;
    /**
     * @brief Sum of all sensor messages
     */
    unsigned int total_nb_messages;
    /**
     * @brief Array containing metrics per sensor
     */
    sensor_message_metrics_t *sensors_metrics;
    unsigned int nb_sensors;
} base_station_metrics_t;

bool init_sensor_metrics(sensor_message_metrics_t *sensor_metrics);
bool init_metrics(base_station_metrics_t *metrics, const unsigned int nb_sensors);
bool dprint_metrics(const int fd, base_station_metrics_t *metrics);
bool free_metrics(base_station_metrics_t *metrics);

#endif // MAKEFILE_METRICS_H
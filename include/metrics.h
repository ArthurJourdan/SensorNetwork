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
    mpi_info_t sensor;
    u_vector_t coordinates;
    unsigned int nb_messages;
    unsigned int nb_alerts;
} sensor_message_metrics_t;

typedef struct
{
    struct tm *start_time;
    /**
     * @brief Sum of all sensors alerts
     */
    unsigned int total_nb_alerts;
    /**
     * @brief Sum of all sensors messages
     */
    unsigned int total_nb_messages;
    /**
     * @brief Array containing metrics per sensor
     */
    sensor_message_metrics_t *sensors_metrics;
} base_station_metrics_t;

#endif // MAKEFILE_METRICS_H
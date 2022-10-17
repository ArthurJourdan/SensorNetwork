/**
 * @file metrics.c
 * @author Arthur Jourdan
 * @date of creation 16/10/22.
 * @brief TODO
 */

#include "metrics.h"

#include <stdlib.h>

bool init_sensor_metrics(sensor_message_metrics_t *sensor_metrics)
{
    sensor_metrics->nb_messages = 0;
    sensor_metrics->nb_alerts = 0;
}

bool init_metrics(base_station_metrics_t *metrics, const unsigned int nb_sensors)
{
    bool retval = true;

    metrics->start_time = time(NULL);
    metrics->total_nb_alerts = 0;
    metrics->total_nb_messages = 0;
    metrics->nb_sensors = nb_sensors;
    if (nb_sensors) {
        metrics->sensors_metrics = malloc(sizeof(sensor_message_metrics_t) * nb_sensors);
        if (!metrics->sensors_metrics)
            return false;
        for (unsigned int i = 0; i < nb_sensors; ++i) {
            if (!init_sensor_metrics(&metrics->sensors_metrics[i]))
                retval = false;
        }
    }
    return retval;
}

bool free_metrics(base_station_metrics_t *metrics)
{
    if (metrics->sensors_metrics) {
        free(metrics->sensors_metrics);
        return true;
    }
    return false;
}
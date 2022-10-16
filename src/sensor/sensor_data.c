/**
 * @file sensor_data.c
 * @author Arthur Jourdan
 * @date of creation 14/10/22.
 * @brief TODO
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "utils.h"
#include "sensor.h"

/**
 * @brief
 * @param data [in]
 */
void print_data(sensor_reading_t *data)
{
    printf(".timestamp =\t%s", asctime(data->timestamp));
    printf(".coordinates =\t");
    print_float_coordinates(data->coordinates);
    printf(".magnitude =\t%f\n", data->magnitude);
    printf(".depth =\t%f\n\n", data->depth);
}

bool generate_geo_coordinates(grid_t *grid, float *latitude, float *longitude)
{
    if (NB_DIMENSIONS < 2)
        return false;
    const float lat_min = -15;  // approximately the ocean on Australia's east coast
    const float lat_max = -45;  // approximately the ocean on Australia's east coast
    const float long_min = 150; // approximately the ocean on Australia's east coast
    const float long_max = 180; // approximately the ocean on Australia's east coast
    const float coo_min_lat =
        lat_min + ((lat_max - lat_min) / (float) grid->size[0]) * ((float) grid->process_position[0]);
    const float coo_max_lat =
        lat_min + ((lat_max - lat_min) / (float) grid->size[0]) * ((float) grid->process_position[0] + 1);
    const float coo_min_long =
        long_min + ((long_max - long_min) / (float) grid->size[1]) * ((float) grid->process_position[1]);
    const float coo_max_long =
        long_min + ((long_max - long_min) / (float) grid->size[1]) * ((float) grid->process_position[1] + 1);

    // Latitude
    *latitude = rand_float(coo_min_lat, coo_max_lat);
    // Longitude
    *longitude = rand_float(coo_min_long, coo_max_long);
    return true;
}

/**
 * @brief
 * @param sensor_cartesian_coordinates [in]
 * @param out [out]
 */
void read_data(grid_t *grid, sensor_reading_t *data)
{
    const time_t act_time = time(NULL);

    convert_time(&act_time, &data->timestamp);
    if (NB_DIMENSIONS == 2)
        generate_geo_coordinates(grid, &data->coordinates[0], &data->coordinates[1]);
    else {
        for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
            data->coordinates[i] = rand_float(0, 180);
        }
    }
    data->magnitude = rand_float(0, 9.5f /*max magnitude*/);
    data->depth = rand_float(0, 700.f);
}

/**
 * @brief
 * @param grid [in]
 * @param data [out]
 * @param data_size [out]
 * @param need_response [in]
 */
bool pack_data(MPI_Comm comm, sensor_reading_t *data, char packed_data[DATA_PACK_SIZE])
{
    int pos = 0; // packed data position

    // timestamp
    if (MPI_Pack(&data->timestamp->tm_sec, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_min, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_hour, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_mday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_mon, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_year, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_wday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_yday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->timestamp->tm_isdst, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (MPI_Pack(&data->coordinates[i], 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
            return false;
    }
    // !coordinates
    if (MPI_Pack(&data->magnitude, 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data->depth, 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, comm) != MPI_SUCCESS)
        return false;
    return true;
}

/**
 * @brief
 * @param grid [in]
 * @param packed_data [in]
 * @param data [out]
 */
bool unpack_data(MPI_Comm comm, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data)
{
    int pos = 0; // packed data position
    const time_t act_time = time(NULL);

    memset(data, 0, sizeof(sensor_reading_t));
    convert_time(&act_time, &data->timestamp);
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_sec, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_min, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_hour, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_mday, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_mon, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_year, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_wday, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_yday, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_isdst, 1, MPI_INT, comm) != MPI_SUCCESS)
        return false;
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &(data->coordinates[i]), 1, MPI_FLOAT, comm) != MPI_SUCCESS)
            return false;
    }
    // !coordinates

    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &(data->magnitude), 1, MPI_FLOAT, comm) != MPI_SUCCESS)
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &(data->depth), 1, MPI_FLOAT, comm) != MPI_SUCCESS)
        return false;
#ifdef DEBUG
    printf("\nUnpacked data :\n");
    print_data(data);
#endif

    return true;
}

static bool copy_sensor_data(sensor_reading_t *src, sensor_reading_t *dest)
{
    dest->timestamp = src->timestamp;
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        dest->coordinates[i] = src->coordinates[i];
    }
    dest->magnitude = src->magnitude;
    dest->depth = src->depth;
    return true;
}

/**
 * @brief save data in history when magnitude is significant ( above 2.5 )
 * @param grid [in,out]
 * @param data [in]
 * @return true is succeed, false otherwise
 */
bool save_data_in_history(grid_t *grid, sensor_reading_t *data)
{
    if (data->magnitude < 2.5f)
        return false;
    grid->data_history = realloc(grid->data_history, sizeof(sensor_reading_t) * (++grid->data_history_size));
    if (!grid->data_history)
        return false;
    copy_sensor_data(data, &grid->data_history[grid->data_history_size - 1]);
    return true;
}
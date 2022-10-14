/**
 * @file sensor.c
 * @author Arthur Jourdan
 * @date of creation 10/09/22.
 * @brief TODO
 */

#include <stdio.h>

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

/**
 * @brief
 * @param out [out]
 */
void read_data(sensor_reading_t *data)
{
    const time_t act_time = time(NULL);

    convert_time(&act_time, &data->timestamp);
    // todo coordinates
    data->magnitude = rand_float(9.5f /*max magnitude*/);
    data->depth = rand_float(700.f);
}

/**
 * @brief
 * @param grid [in]
 * @param data [out]
 * @param data_size [out]
 */
bool read_pack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE])
{
    sensor_reading_t data;
    int pos = 0; // packed data position

    read_data(&data);

    // timestamp
    if (MPI_Pack(&data.timestamp->tm_sec, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_min, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_hour, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_mday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_mon, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_year, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_wday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_yday, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_isdst, 1, MPI_INT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (MPI_Pack(&data.coordinates[i], 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
            return false;
    }
    // !coordinates
    if (MPI_Pack(&data.magnitude, 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.depth, 1, MPI_FLOAT, packed_data, DATA_PACK_SIZE, &pos, grid->comm) != MPI_SUCCESS)
        return false;
    return true;
}

/**
 * @brief
 * @param grid [in]
 * @param packed_data [in]
 * @param data [out]
 */
bool unpack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data)
{
    int pos = 0; // packed data position
    const time_t act_time = time(NULL);

    convert_time(&act_time, &data->timestamp);
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_sec, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_min, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_hour, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_mday, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_mon, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_year, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_wday, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_yday, 1, MPI_INT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &data->timestamp->tm_isdst, 1, MPI_INT, grid->comm))
        return false;
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (MPI_Unpack(
                packed_data, DATA_PACK_SIZE, &pos, &(data->coordinates[NB_DIMENSIONS]), 1, MPI_FLOAT, grid->comm))
            return false;
    }
    // !coordinates

    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &(data->magnitude), 1, MPI_FLOAT, grid->comm))
        return false;
    if (MPI_Unpack(packed_data, DATA_PACK_SIZE, &pos, &(data->depth), 1, MPI_FLOAT, grid->comm))
        return false;
#ifdef DEBUG
    printf("\nUnpacked data :\n");
    print_data(data);
#endif
    return true;
}
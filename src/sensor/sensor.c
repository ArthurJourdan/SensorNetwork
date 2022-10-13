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
    printf(".timestamp =\t%s.\n", asctime(data->timestamp));
    printf(".coordinates =\t");
    print_float_coordinates(data->coordinates);
    printf(".magnitude =\t%f.\n", data->magnitude);
    printf(".depth =\t%f.\n", data->depth);
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
    int packed_data_position = 0;

    read_data(&data);

    // timestamp
    if (MPI_Pack(&data.timestamp->tm_sec,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_min,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_hour,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_mday,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_mon,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_year,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_wday,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_yday,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.timestamp->tm_isdst,
            1,
            MPI_INT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(int) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        if (MPI_Pack(&data.coordinates[i],
                1,
                MPI_FLOAT,
                packed_data,
                DATA_PACK_SIZE /*(sizeof(float) + packed_data_position)*/,
                &packed_data_position,
                grid->comm)
            != MPI_SUCCESS)
            return false;
    }
    // !coordinates
    if (MPI_Pack(&data.magnitude,
            1,
            MPI_FLOAT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(float) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    if (MPI_Pack(&data.depth,
            1,
            MPI_FLOAT,
            packed_data,
            DATA_PACK_SIZE /*(sizeof(float) + packed_data_position)*/,
            &packed_data_position,
            grid->comm)
        != MPI_SUCCESS)
        return false;
    return true;
}

#include "unistd.h"

/**
 * @brief
 * @param grid [in]
 * @param packed_data [in]
 * @param data [out]
 */
void unpack_data(grid_t *grid, char packed_data[DATA_PACK_SIZE], sensor_reading_t *data)
{
    int packed_data_position = 0;

    // timestamp
    printf("%s.\n", "hello1");
    //    write(1, packed_data, DATA_PACK_SIZE);
    //    printf(".\n");
    MPI_Unpack(packed_data,
        DATA_PACK_SIZE /*sizeof(int)*/,
        &packed_data_position,
        &data->timestamp->tm_sec,
        1,
        MPI_INT,
        grid->comm);
    printf("%s.\n", "hello2");
    MPI_Unpack(packed_data,
        DATA_PACK_SIZE /*sizeof(int)*/,
        &packed_data_position,
        &data->timestamp->tm_min,
        1,
        MPI_INT,
        grid->comm);
    printf("%s.\n", "hello3");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_hour, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello4");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_mday, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello5");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_mon, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello6");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_year, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello7");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_wday, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello8");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_yday, 1, MPI_INT, grid->comm);
    printf("%s.\n", "hello9");
    MPI_Unpack(packed_data, sizeof(int), &packed_data_position, &data->timestamp->tm_isdst, 1, MPI_INT, grid->comm);
    // !timestamp

    // coordinates
    for (unsigned int i = 0; i < NB_DIMENSIONS; ++i) {
        MPI_Unpack(packed_data,
            sizeof(float),
            &packed_data_position,
            &data->coordinates[NB_DIMENSIONS],
            1,
            MPI_FLOAT,
            grid->comm);
    }
    // !coordinates

    MPI_Unpack(packed_data, sizeof(float), &packed_data_position, &data->magnitude, 1, MPI_FLOAT, grid->comm);
    MPI_Unpack(packed_data, sizeof(float), &packed_data_position, &data->depth, 1, MPI_FLOAT, grid->comm);
    printf("%s.\n", "recevied : ");
    print_data(data);
}
/**
 * @file sensor_data_cmp_distance.c
 * @author Arthur Jourdan
 * @date of creation 15/10/22.
 * @brief TODO
 */


// The following code is taken from https://www.geodatasource.com/developers/c
// The only changes made is the change from double type to float,
//  changing sin to sinf, cos to cosf and acos to acosf

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::                                                                         :*/
/*::  This routine calculates the distance between two points (given the     :*/
/*::  latitude/longitude of those points). It is being used to calculate     :*/
/*::  the distance between two locations using GeoDataSource(TM) products.   :*/
/*::                                                                         :*/
/*::  Definitions:                                                           :*/
/*::    South latitudes are negative, east longitudes are positive           :*/
/*::                                                                         :*/
/*::  Passed to function:                                                    :*/
/*::    lat1, lon1 = Latitude and Longitude of point 1 (in decimal degrees)  :*/
/*::    lat2, lon2 = Latitude and Longitude of point 2 (in decimal degrees)  :*/
/*::    unit = the unit you desire for results                               :*/
/*::           where: 'M' is statute miles (default)                         :*/
/*::                  'K' is kilometers                                      :*/
/*::                  'N' is nautical miles                                  :*/
/*::  Worldwide cities and other features databases with latitude longitude  :*/
/*::  are available at https://www.geodatasource.com                         :*/
/*::                                                                         :*/
/*::  For enquiries, please contact sales@geodatasource.com                  :*/
/*::                                                                         :*/
/*::  Official Web site: https://www.geodatasource.com                       :*/
/*::                                                                         :*/
/*::           GeoDataSource.com (C) All Rights Reserved 2022                :*/
/*::                                                                         :*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

#include <math.h>

#define pi 3.14159265358979323846

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Function prototypes                                           :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
float deg2rad(float);
float rad2deg(float);

float distance(float lat1, float lon1, float lat2, float lon2, char unit)
{
    float theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else {
        theta = lon1 - lon2;
        dist =
            sinf(deg2rad(lat1)) * sinf(deg2rad(lat2)) + cosf(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
        dist = acosf(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.1515;
        switch (unit) {
            case 'M': break;
            case 'K': dist = dist * 1.609344; break;
            case 'N': dist = dist * 0.8684; break;
        }
        return (dist);
    }
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
float deg2rad(float deg)
{
    return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
float rad2deg(float rad)
{
    return (rad * 180 / pi);
}

// End of code taken from https://www.geodatasource.com/developers/c
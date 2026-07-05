#include "raptor_util.h"

double haversine(double angle) {
    return (1 - cos(angle))/2;
}
double degreeToRadian(double angle) {
    return M_PI * angle / 180;
}

const double EARTH_RADIUS = 6371;
double getDistance(double a_lat, double a_lon, double b_lat, double b_lon) {
    a_lat = degreeToRadian(a_lat);
    a_lon = degreeToRadian(a_lon);
    b_lat = degreeToRadian(b_lat);
    b_lon = degreeToRadian(b_lon);
    double delta_lat = b_lat - a_lat;
    double delta_lon = b_lon - a_lon;

    double havTheta = haversine(delta_lat) + cos(a_lat)*cos(b_lat)*haversine(delta_lon);

    double inversed = 2*asin(sqrt(havTheta));
    return EARTH_RADIUS * inversed;
}



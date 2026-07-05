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

//in km/s, to match units
//this is 2m/s, slower than actual walking speed
//but it assumes that you can walk in a straight line between stops
//which is not true, so we account for BUILDINGS AND STUFF
double walkingSpeed = 0.002;
vector<vector<seconds>> getWalks(vector<stops> &stopsTable) {
    int size = stopsTable.size();
    vector<vector<seconds>> result(size, vector<seconds>(size, 0));
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            result[i][j] = getDistance(stopsTable[i].stop_lat, stopsTable[i].stop_lon,
                stopsTable[j].stop_lat, stopsTable[j].stop_lon) / walkingSpeed;
        }
    }
    return result;
}


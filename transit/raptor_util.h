#ifndef RAPTOR_UTIL_H
#define RAPTOR_UTIL_H

#include "parser.h"

// arguments in degrees (just as in GTFS), result in kilometers
double getDistance(double a_lat, double a_lon, double b_lat, double b_lon);

//rough estimation based purely on distance
vector<vector<seconds>> getWalks(vector<stops> &stopsTable);

#endif // RAPTOR_UTIL_H
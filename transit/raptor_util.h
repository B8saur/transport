#ifndef RAPTOR_UTIL_H
#define RAPTOR_UTIL_H

#include "parser.h"

struct routes {
    vector<int> stopsList;          //stop_sequence-1 -> stopIdx
    vector<seconds> between;        //how much time does it take to go from i-th to (i+1)-th stop
    vector<seconds> startTimes;     //times when it (bus etc) departs from the first stop
};

// arguments in degrees (just as in GTFS), result in kilometers
double getDistance(double a_lat, double a_lon, double b_lat, double b_lon);

//rough estimation based purely on distance
vector<vector<seconds>> getWalks(vector<stops> &stopsTable);

vector<routes> getRoutes(vector<stops> &stopsTable, vector<times> &timesTable, vector<trips> &tripsTable);

#endif // RAPTOR_UTIL_H
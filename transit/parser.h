#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long seconds;

struct stops {          //only for distances between stops
    string stop_id;                 //stop_1_1
    double stop_lat;                //50.0873
    double stop_lon;                //20.0596
};

struct times {          //most important
    string trip_id;                 //block_2_trip_2_service_5
    seconds arrival_time;           //05:03:00
    seconds departure_time;         //05:03:00
    string stop_id;                 //stop_213_31119
    int stop_sequence;              //1                             //then 2, 3, 4, ...
    double shape_dist_traveled;     //0                             //distance from start (point) of the trip
};

struct trips {
    string trip_id;                 //block_2_trip_2_service_5
    string route_id;                //route_31                      //trips with the same route_id have the same list of stops
    string service_id;              //service_5                     //determine day of the week
};


vector<stops> getStops();
vector<times> getTimes();
vector<trips> getTrips();


#endif // PARSER_H
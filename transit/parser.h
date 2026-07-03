#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long seconds;

vector<vector<string>> parseFile(string filename);


const string fileTimes = "data/stop_times.txt";
const string fileStops = "data/stops.txt";
const string fileTrips = "data/trips.txt";

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
    string service_id;              //service_5                     //determine day of the week
};

#endif // PARSER_H
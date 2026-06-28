#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> parseFile(string filename);


const string fileRoutes = "routes.txt";
const string fileShapes = "shapes.txt";
const string fileTimes = "stop_times.txt";
const string fileStops = "stops.txt";
const string fileTrips = "trips.txt";

/*
// struct routes {
    // string route_id;                //route_31
    // string agency_id;               //agency_1                      //the same everywhere
    // string route_short_name;        //"1"
    // string route_long_name;         //"1"
    // string route_desc;              //
    // string route_type;              //900
    // string route_url;               //
    // string route_color;             //
    // string route_text_color;        //
// };

struct shapes {                     //generally boring
    string shape_id;                //shape_12
    string shape_pt_lat;            //50.031724
    string shape_pt_lon;            //19.934085
    string shape_pt_sequence;       //1
    // string shape_dist_traveled;     //                                  //no value
};

struct times {
    string trip_id;                 //block_2_trip_2_service_5
    string arrival_time;            //05:03:00
    string departure_time;          //05:03:00
    string stop_id;                 //stop_213_31119
    string stop_sequence;           //1
    // string stop_headsign;           //""
    // string pickup_type;             //0
    // string drop_off_type;           //1
    string shape_dist_traveled;     //0                             //distance from start (point) of the trip
    // string timepoint;               //1
};

struct stops {
    string stop_id;                 //stop_1_1
    // string stop_code;               //920-01                        //unique
    // string stop_name;               //"Grodzki Urząd Pracy"
    // string stop_desc;               //01                            //usually like "01", maybe error
    string stop_lat;                //50.0873
    string stop_lon;                //20.0596
    // string zone_id;                 //
    // string stop_url;                //
    // string location_type;           //0                             //always 0
    // string parent_station;          //
    // string stop_timezone;           //
    // string wheelchair_boarding;     //
    // string platform_code;           //
};

struct trips {
    string trip_id;                 //block_2_trip_2_service_5
    string route_id;                //route_31
    string service_id;              //service_5                     //determine day of the week
    // string trip_headsign;           //Wzg. Krzesławickie
    // string trip_short_name;         //""
    string direction_id;            //0
    string block_id;                //block_2
    string shape_id;                //shape_95
    // string wheelchair_accessible;   //0
};
*/





struct stops {          //only for distances between stops
    string stop_id;                 //stop_1_1
    string stop_lat;                //50.0873
    string stop_lon;                //20.0596
};

struct times {          //most important
    string trip_id;                 //block_2_trip_2_service_5
    string arrival_time;            //05:03:00
    string departure_time;          //05:03:00
    string stop_id;                 //stop_213_31119
    string stop_sequence;           //1                             //then 2, 3, 4, ...
    string shape_dist_traveled;     //0                             //distance from start (point) of the trip
};

struct trips {
    string trip_id;                 //block_2_trip_2_service_5
    string service_id;              //service_5                     //determine day of the week
    string direction_id;            //0
};


#endif // PARSER_H
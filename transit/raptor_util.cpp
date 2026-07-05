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

string getMostCommonService(vector<trips> &tripsTable) {
    //yes, this code is completely copied
    unordered_map<string, int> countServices;
    for(auto &trip : tripsTable) {
        countServices[trip.service_id] += 1;
    }

    int bestCount = 0;
    string bestName;
    for(auto [name, count] : countServices) {
        if(count > bestCount) {
            bestCount = count;
            bestName = name;
        }
    }
    return bestName;
}

vector<routes> getRoutes(vector<stops> &stopsTable, vector<times> &timesTable, vector<trips> &tripsTable) {
    unordered_map<string, int> stopIdx;
    for(int i=0; i<stopsTable.size(); i++) {
        stopIdx[stopsTable[i].stop_id] = i;
    }

    unordered_map<string, int> routeIdx;        //updated on the fly

    string curService = getMostCommonService(tripsTable);
    unordered_set<string> usedRoutes;

    int curTimesIdx = 0;
    vector<routes> result;
    for(auto &trip : tripsTable) {
        if(trip.service_id != curService) {         //wrong service
            while(curTimesIdx < timesTable.size() && timesTable[curTimesIdx].trip_id == trip.trip_id) {
                curTimesIdx += 1;
            }
            continue;
        }
        if(!usedRoutes.count(trip.route_id)) {           //new route - add stops and startTimes
            usedRoutes.insert(trip.route_id);
            routeIdx[trip.route_id] = result.size();
            result.resize(result.size()+1);         //make space for new route
            auto &curRoute = result.back();

            curRoute.startTimes.push_back(timesTable[curTimesIdx].arrival_time);            //first start time
            curRoute.stopsList.push_back(stopIdx[timesTable[curTimesIdx].stop_id]);         //first stop

            seconds prevTime = timesTable[curTimesIdx].arrival_time;
            curTimesIdx += 1;   //start from next and compare to prev
            while(curTimesIdx < timesTable.size() && timesTable[curTimesIdx].trip_id == trip.trip_id) {
                curRoute.stopsList.push_back(stopIdx[timesTable[curTimesIdx].stop_id]);     //other stops
                curRoute.between.push_back(timesTable[curTimesIdx].arrival_time - prevTime);    //all between-times
                prevTime = timesTable[curTimesIdx].arrival_time;
                curTimesIdx += 1;
            }
            //cheap trick - make it 0-ended
            curRoute.between.push_back(0);
        }
        else {          //known route, just add another start time
            auto &curRoute = result[routeIdx[trip.route_id]];
            curRoute.startTimes.push_back(timesTable[curTimesIdx].arrival_time);            //new start time
            
            //again - skip to the next trip
            while(curTimesIdx < timesTable.size() && timesTable[curTimesIdx].trip_id == trip.trip_id) {
                curTimesIdx += 1;
            }
        }
    }

    for(auto &curRoute : result) {          //make sure that these are in order
        sort(curRoute.startTimes.begin(), curRoute.startTimes.end());
    }

    return result;
}
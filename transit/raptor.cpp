#include "raptor_util.h"

//minimal time between arrival and departure from a stop IF you change the trip
const seconds changeTime = 179;

void printTime(seconds curTime) {
    seconds h = curTime/3600;   //seconds in an hour
    seconds m = (curTime%3600)/60;
    seconds s = curTime%60;
    cout.width(2);
    cout.fill('0');
    cout << h << ":";
    cout.width(2);
    cout.fill('0');
    cout << m << ":";
    cout.width(2);
    cout.fill('0');
    cout << s;
}

int main() {
    vector<stops> stopsTable = getStops();
    vector<times> timesTable = getTimes();
    vector<trips> tripsTable = getTrips();
    vector<routes> allRoutes = getRoutes(stopsTable, timesTable, tripsTable);

    for(auto &route : allRoutes) {
        cout << "    " << route.stopsList.size() << "\n";
        for(auto stopId : route.stopsList) {
            cout << stopId << " -> ";
        }
        cout << "\n";

        cout << "    " << route.startTimes.size() << "\n";
        for(auto start : route.startTimes) {
            printTime(start);
            cout << " ";
        }
        cout << "\n";

        cout << "    " << route.between.size() << "\n";
        for(auto betweenTime : route.between) {
            cout << " " << betweenTime;
        }
        cout << "\n";

        cout << "############\n";
    }
}

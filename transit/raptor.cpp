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
    cout << "Loading data...\n";
    cout.flush();
    vector<stops> stopsTable = getStops();
    vector<times> timesTable = getTimes();
    vector<trips> tripsTable = getTrips();

    vector<routes> allRoutes = getRoutes(stopsTable, timesTable, tripsTable);
    cout << "All data loaded\n";
    cout.flush();

    cout << "Choose start, end and start time measured in seconds from midnight (86400 is a whole day)\n";
    cout << "Stops range from " << 0 << " to " << stopsTable.size()-1 << "\n";
    cout << "Ex. 21 37 36000\n";
    cout.flush();

    int start, end;
    seconds startTime;
    while(true) {
        cout << "###:  ";
        cout.flush();
        cin >> start >> end >> startTime;
        if(start < 0 || start >= stopsTable.size() || end < 0 || end >= stopsTable.size()) {
            cout << "Chosen stops are out of range, try again\n";
            cout.flush();
            continue;
        }
        cout << "Start time is ";
        printTime(startTime);
        cout << "\nCalculating...\n";
        cout.flush();

        //TODO: link actual function
        seconds endTime = startTime;

        if(endTime == NEVER) {
            cout << "Sorry, no connection here\n";
        }
        else {
            cout << "Earliest arrival found at " << endTime << " = ";
            printTime(endTime);
            cout << "\n";
        }
        cout << "Try again\n";
        cout.flush();
    }
    return 0;
}

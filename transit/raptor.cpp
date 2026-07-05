#include "raptor_util.h"

//minimal time between arrival and departure from a stop IF you change the trip
const seconds changeTime = 179;


seconds doRaptor(vector<routes> &allRoutes, vector<vector<seconds>> &allWalks, int stopBegin, int stopEnd, int roundCount, seconds startTime) {
    int stopsCount = allWalks.size();
    vector<seconds> arrivalTimes(stopsCount, NEVER);
    arrivalTimes[stopBegin] = startTime;
    
    //walks done even before the algorithm
    for(int i=0; i<stopsCount; i++) {
        for(int j=0; j<stopsCount; j++) {
            arrivalTimes[j] = min(arrivalTimes[j], arrivalTimes[i] + allWalks[i][j]);
        }
    }

    seconds changePenalty = 0;          //usually equal to `changeTime`, but first round does not have this penalty
    for(int round=0; round<roundCount; round++) {
        vector<seconds> newTimes = arrivalTimes;
        bool updatedAny = false;

        //routes
        for(auto &route : allRoutes) {
            int earliestTrip = route.startTimes.size();

            seconds offset = 0;         //between start time and actual time at the stop
            seconds earliestStart = NEVER;
            for(int i=0; i<route.stopsList.size(); offset += route.between[i++]) {      //please notice the "i++"
                int stop = route.stopsList[i];

                //acutally find earliest trip (and earliest start)
                while(earliestTrip > 0 && route.startTimes[earliestTrip-1] + offset >= arrivalTimes[stop] + changePenalty) {    //previous trip (exists and) fits
                    earliestTrip -= 1;
                    earliestStart = route.startTimes[earliestTrip];
                }

                //update if possible
                if(earliestStart + offset < newTimes[stop]) {
                    updatedAny = true;
                    newTimes[stop] = earliestStart + offset;
                }
            }
        }

        //walks
        for(int i=0; i<stopsCount; i++) {
            for(int j=0; j<stopsCount; j++) {
                newTimes[j] = min(newTimes[j], newTimes[i] + allWalks[i][j]);
            }
        }

        if(updatedAny == false)         //did not update anything - just return
            break;

        swap(arrivalTimes, newTimes);       //copy updated times (but simpler)

        //it only needs to be done once, but that doesn't hurt
        changePenalty = changeTime;
    }

    return arrivalTimes[stopEnd];
}


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
    vector<vector<seconds>> allWalks = getWalks(stopsTable);
    cout << "All data loaded\n";
    cout.flush();

    cout << "Choose start, end, max trips used andstart time measured in seconds from midnight (86400 is a whole day)\n";
    cout << "Stops range from " << 0 << " to " << stopsTable.size()-1 << "\n";
    cout << "Ex. 21 37 5 36000\n";
    cout.flush();

    int start, end, roundCount;
    seconds startTime;
    while(true) {
        cout << "###:  ";
        cout.flush();
        cin >> start >> end >> roundCount >> startTime;
        if(start < 0 || start >= stopsTable.size() || end < 0 || end >= stopsTable.size()) {
            cout << "Chosen stops are out of range, try again\n";
            cout.flush();
            continue;
        }
        cout << "Start time is ";
        printTime(startTime);
        cout << "\nCalculating...\n";
        cout.flush();

        //actually do the thing
        seconds endTime = doRaptor(allRoutes, allWalks, start, end, roundCount, startTime);

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

#include "parser.h"

//minimal time between arrival and departure from a stop IF you change the trip
const seconds changeTime = 180;

//choose the most common service and select trips of that service
unordered_set<string> selectTrips(vector<trips> &tripsTable) {
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

    unordered_set<string> result;
    for(auto &trip : tripsTable) {
        if(trip.service_id == bestName) {
            result.insert(trip.trip_id);
        }
    }
    return result;
}

//doesn't actually sort, returns table of indexes ordered by departure_time
vector<int> sortTimesByDepartureTime(vector<times> &timesTable, vector<trips> &tripsTable) {
    unordered_set<string> chosenTrips = selectTrips(tripsTable);
    vector<int> result;
    for(int i=0; i<timesTable.size(); i++) {
        if(chosenTrips.count(timesTable[i].trip_id)) {          //one of the chosen trips
            result.push_back(i);
        }
    }

    sort(result.begin(), result.end(), [&timesTable](int a, int b){
        return timesTable[a].departure_time < timesTable[b].departure_time;
    });

    return result;
}

//node naming cheat sheet:
//
//    #->------------>-#
//    ^                |
//    |                v
// -> in   central -> out ->
//       \    |
//        v   v
// -> in   central -> out ->
//...
//edge types:
//1. ride: out -> in
//2. continue: in -> out
//3. change: in -> central
//4. wait: central -> central
//5. board: central -> out
const int NONODE = -1;
struct Node {
    seconds curTime;
    int stopIdx;
    int nextCentral = NONODE;
    int nextOther = NONODE;         //in or out type
    Node(seconds curTime, int stopIdx) : curTime(curTime), stopIdx(stopIdx){}
};
struct TimeExpandedGraph {
    //stop_id -> index in nodes of the first central node of that stop
    unordered_map<string, int> primaryStopNodes;
    vector<Node> nodes;
};

TimeExpandedGraph buildGraph(vector<times> &timesTable, vector<int> &timeOrdered, vector<stops> &stopsTable) {
    TimeExpandedGraph graph;
    unordered_map<string, int> lastSeenOnStop;          //stop_id -> index in nodes, central node
    unordered_map<string, int> stopIdx;
    for(int i=0; i<stopsTable.size(); i++) {
        stopIdx[stopsTable[i].stop_id] = i;
    }

    unordered_map<string, int> prevTripOutNode;         //trip_id -> index in nodes
    unordered_map<int, int> prevCentralNode;            //central node id -> central node id of the previous node
    for(auto curIdx : timeOrdered) {
        string &curStop = timesTable[curIdx].stop_id;
        int curStopIdx = stopIdx[curStop];

        //central node
        int centralNodeIdx = graph.nodes.size();
        graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
        if(!lastSeenOnStop.count(curStop)) {        //completely new stop
            graph.primaryStopNodes[curStop] = centralNodeIdx;
            prevCentralNode[centralNodeIdx] = NONODE;
        }
        else {
            graph.nodes[lastSeenOnStop[curStop]].nextCentral = centralNodeIdx;          //edge type 4
            prevCentralNode[centralNodeIdx] = lastSeenOnStop[curStop];
        }
        lastSeenOnStop[curStop] = centralNodeIdx;

        //out node
        int outNodeIdx = graph.nodes.size();
        graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
        graph.nodes[centralNodeIdx].nextOther = outNodeIdx;         //edge type 5

        //in node
        if(timesTable[curIdx].stop_sequence != 1) {         //not the start - something actually arrives and there's previous out node
            int inNodeIdx = graph.nodes.size();
            graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
            graph.nodes[inNodeIdx].nextOther = outNodeIdx;          //edge type 2

            int prevOutNode = prevTripOutNode[timesTable[curIdx].trip_id];
            graph.nodes[prevOutNode].nextOther = inNodeIdx;         //edge type 1
        }
        
        //back to out node
        prevTripOutNode[timesTable[curIdx].trip_id] = outNodeIdx;
    }

    //TODO: loop for edges type 3, using `prevCentralNode`
}

int main() {
    vector<stops> stopsTable = getStops();
    vector<times> timesTable = getTimes();
    vector<trips> tripsTable = getTrips();

    //TODO: interface
    vector<int> timeOrdered = sortTimesByDepartureTime(timesTable, tripsTable);
    for(auto a : timeOrdered)
        cout << a << " " << timesTable[a].departure_time <<  "\n";
}

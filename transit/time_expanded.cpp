#include "parser.h"

//minimal time between arrival and departure from a stop IF you change the trip
const seconds changeTime = 179;

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
    //stop index -> index in nodes of the first central node of that stop
    vector<int> primaryStopNodes;
    vector<Node> nodes;
};

TimeExpandedGraph buildGraph(vector<times> &timesTable, vector<int> &timeOrdered, vector<stops> &stopsTable) {
    TimeExpandedGraph graph;
    graph.primaryStopNodes.resize(stopsTable.size());
    unordered_map<string, int> lastSeenOnStop;          //stop_id -> index in nodes, central node
    unordered_map<string, int> stopIdx;
    for(int i=0; i<stopsTable.size(); i++) {
        stopIdx[stopsTable[i].stop_id] = i;
    }

    unordered_map<string, int> prevTripOutNode;         //trip_id -> index in nodes
    vector<int> allInNodes;         //indexes in nodes of all in nodes
    for(auto curIdx : timeOrdered) {
        string &curStop = timesTable[curIdx].stop_id;
        int curStopIdx = stopIdx[curStop];

        //central node
        int centralNodeIdx = graph.nodes.size();
        graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
        if(!lastSeenOnStop.count(curStop)) {        //completely new stop
            graph.primaryStopNodes[stopIdx[curStop]] = centralNodeIdx;
        }
        else {
            graph.nodes[lastSeenOnStop[curStop]].nextCentral = centralNodeIdx;          //edge type 4
        }
        lastSeenOnStop[curStop] = centralNodeIdx;

        //out node
        int outNodeIdx = graph.nodes.size();
        graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
        graph.nodes[centralNodeIdx].nextOther = outNodeIdx;         //edge type 5

        //in node
        if(timesTable[curIdx].stop_sequence != 1) {         //not the start - something actually arrives and there's previous out node
            int inNodeIdx = graph.nodes.size();
            allInNodes.push_back(inNodeIdx);
            graph.nodes.emplace_back(timesTable[curIdx].departure_time, curStopIdx);
            graph.nodes[inNodeIdx].nextOther = outNodeIdx;          //edge type 2

            //prev out  to  cur in
            int prevOutNode = prevTripOutNode[timesTable[curIdx].trip_id];
            graph.nodes[prevOutNode].nextOther = inNodeIdx;         //edge type 1
        }
        
        //back to out node
        prevTripOutNode[timesTable[curIdx].trip_id] = outNodeIdx;
    }

    for(int inNode : allInNodes) {      //create type 3 edges
        //nodes are always created in order: central, out, [in]
        //so each in node has its central node at `x-2`
        int centralNode = inNode-2;
        while(centralNode != NONODE && graph.nodes[centralNode].curTime - graph.nodes[inNode].curTime < changeTime) {
            centralNode = graph.nodes[centralNode].nextCentral;
        }
        graph.nodes[inNode].nextCentral = centralNode;      //edge type 3
    }

    return graph;
}

seconds dijkstra(TimeExpandedGraph &graph, int stopBegin, int stopEnd, seconds startTime) {
    int startNode = graph.primaryStopNodes[stopBegin];
    while(startNode != NONODE && startTime > graph.nodes[startNode].curTime) {
        startNode = graph.nodes[startNode].nextCentral;
    }

    if(startNode == NONODE) {
        return NEVER;
    }

    auto comp = [&graph](int a, int b) {
        return graph.nodes[a].curTime > graph.nodes[b].curTime;
    };
    priority_queue<int, vector<int>, decltype(comp)> pq(comp);
    vector<bool> visited(graph.nodes.size(), false);        //slight overkill
    visited[startNode] = true;
    pq.push(startNode);

    while(!pq.empty()) {
        int cur = pq.top();
        pq.pop();

        //find result
        if(graph.nodes[cur].stopIdx == stopEnd) {
            return graph.nodes[cur].curTime;
        }

        //add nexts
        int next = graph.nodes[cur].nextCentral;
        if(next != NONODE && visited[next] == false) {
            pq.push(next);
            visited[next] = true;
        }
        next = graph.nodes[cur].nextOther;
        if(next != NONODE && visited[next] == false) {
            pq.push(next);
            visited[next] = true;
        }
    }

    return NEVER;
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

    vector<int> timeOrdered = sortTimesByDepartureTime(timesTable, tripsTable);
    TimeExpandedGraph graph = buildGraph(timesTable, timeOrdered, stopsTable);
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

        seconds endTime = dijkstra(graph, start, end, startTime);

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

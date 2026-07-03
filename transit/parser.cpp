#include "parser.h"


vector<string> splitLine(string line, char delimiter = ',') {
    line.back() = delimiter;
    vector<string> result;
    int prev = 0;
    for(int i=prev; i<line.size(); i++) {
        if(line[i] != delimiter)
            continue;

        result.push_back(line.substr(prev, i-prev));
        prev = i+1;
    }
    return result;
}

vector<vector<string>> parseFile(const string filename) {
    ifstream currentFile(filename);

    vector<vector<string>> result;

    string line;
    getline(currentFile, line);         //first line of the file is boring - contains headers, not data
    while(getline(currentFile, line)) {
        vector<string> afterSplit = splitLine(line);
        result.push_back(afterSplit);
    }
    return result;
}

vector<stops> getStops() {
    vector<vector<string>> data = parseFile(fileStops);
    vector<stops> result(data.size());
    for(int i=0; i<data.size(); i++) {
        result[i].stop_id = data[i][0];
        result[i].stop_lat = stod(data[i][4]);
        result[i].stop_lon = stod(data[i][5]);
    }
    return result;
}

seconds convertTime(string curTime) {
    seconds result = 0;
    bool prevNum = false;
    for(auto c : curTime) {
        if(c == ':') {
            result *= 6;
            prevNum = false;
        }
        else {
            result += (c-'0');
            if(prevNum == false) {
                prevNum = true;
                result *= 10;
            }
        }
    }
    return result;
}
vector<times> getTimes() {
    vector<vector<string>> data = parseFile(fileTimes);
    vector<times> result(data.size());
    for(int i=0; i<data.size(); i++) {
        result[i].trip_id = data[i][0];
        result[i].arrival_time = convertTime(data[i][1]);
        result[i].departure_time = convertTime(data[i][2]);
        result[i].stop_id = data[i][3];
        result[i].stop_sequence = stoi(data[i][4]);
        result[i].shape_dist_traveled = stod(data[i][8]);
    }
    return result;
}

vector<trips> getTrips() {
    vector<vector<string>> data = parseFile(fileTrips);
    vector<trips> result(data.size());
    for(int i=0; i<data.size(); i++) {
        result[i].trip_id = data[i][0];
        result[i].service_id = data[i][2];
    }
    return result;
}


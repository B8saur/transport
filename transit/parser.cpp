#include "parser.h"
using namespace std;


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

vector<vector<string>> parseFile(string filename) {
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



int main() {
    string filename = "cracow_trams/routes.txt";

    vector<vector<string>> data = parseFile(filename);

    for(int j=0; j<data.size(); j++) {
        for(int i=0; i<data[j].size(); i++) {
            cout << j << " " << i << ":  @" << data[j][i] << "@\n";
            // cout << i << ":  \n";
            // cout << "@" << v[i] << "@\n";
            // for(auto c : v[i])
            //     cout << (int)c << "_";
            // cout << "\n";
        }
        cout << "############\n";
    }
}

#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <list>

using namespace std;

void translate(vector<vector<int>>& g, string dir) {
    ofstream file(dir);

    file << "c assignment 3" << endl;
    file << "c" << endl;
    file << "p edge " << g.size() << " " << g.size() << endl;

    for(int i = 0; i < g.size(); i++) {
        for(int j = 0; j < g.size(); j++) {
            if(g[i][j])
                file << "e " << i+1 << " " << j+1 << endl;
        }
    }

    file.close();
}

vector< vector<int> > fileToGraph(string filename) {
    
    ifstream file(filename);
    int nodes = 0;
    string line;

    while(getline(file, line)) {
        if(line[0] != 'p') continue;
        char *token = std::strtok(const_cast<char*>(line.c_str()), " ");
        token = std::strtok(nullptr, " ");
        token = std::strtok(nullptr, " ");
        nodes = stoi(token);
        break;
    }

    vector< vector<int> > graph(nodes, vector<int>(nodes));
    
    while(getline(file, line)) {
        if(line[0] != 'e') continue;
        char *token = std::strtok(const_cast<char*>(line.c_str()), " "); // e;
        token = std::strtok(nullptr, " "); // node 1
        int a = stoi(token)-1;
        token = std::strtok(nullptr, " "); // node 2
        int b = stoi(token)-1;
        
        graph[a][b] = 1; graph[b][a] = 1;
    }

    return graph;
}

#endif
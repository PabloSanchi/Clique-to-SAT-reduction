#ifndef REDUCTION_HPP
#define REDUCTION_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

int initialize(vector< vector<int> >& x, size_t n, size_t k){
	
	int var = 1;

	for(size_t i = 0; i < n; i++){
		for(size_t r = 0; r < k; r++){
			x[i][r] = var++;
		}
	}
	
	return var;
}

void graphTOCNF(vector< vector<int> > g, size_t k) {

	ofstream file("output.cnf");
	file<<"c Reduction to CNF\n";
	
	
	int n = g.size();
	vector< vector<int> > num(g.size(), vector<int>(k,0));
	int numOfVar = initialize(num, g.size(), k);
	size_t numOfClauses = 0;
    string text = "";
	
	// P1) Each vertex in a clique must be a vertex in the graph
	for(size_t r = 0; r < k; r++){
		for(size_t i = 0; i < g.size(); i++){
			text += to_string(num[i][r]) + " ";
		}
		text += "0\n";
		++numOfClauses;
	}

    // P2) A vertex cannot appear twice in the same clique
    for(size_t i = 0; i < g.size(); i++){
		for(size_t s = 1; s < k; s++){
			for(size_t r = 0; r < s; r++){
				text += to_string(num[i][r]*(-1)) 
                    + " " + to_string(num[i][s]*(-1))
                    + " 0\n";
				++numOfClauses;
			}
		}
	}

    // P3) Two nonadjacent vertices cannot be in the same clique
	for(size_t j = 1; j < n; ++j) {
        for(size_t i = 0; i < j; ++i) {
            if(g[j][i]) continue;
            for(size_t r = 0; r < k; ++r) {
                for(size_t s = 0; s < k; ++s) {
                    if(r != s) {	
						text += to_string(num[i][r]*(-1))
                                + " " + to_string(num[j][s]*(-1)) + " 0\n";
						++numOfClauses;	
					}
                }
            }
        }
    }

    file << "p cnf " << numOfVar-1 << " " << numOfClauses << endl;
    file << text;
	file.close();
}

#endif
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include "reduction.hpp"
#include "translator.hpp"

using namespace std;

/**
 * @brief Create a subgraph from an already existing graph
 *        it will take only the first n nodes
 * 
 * @param g 
 * @param allowedVertex 
 * @param n 
 * @return vector<vector<int>> 
 */
vector<vector<int>> createGraph(vector<vector<int>> g, vector<int> allowedVertex, size_t n) {
    vector<vector<int>> v(g.size(), vector<int>(g.size(), 0));

    for(auto v1 = allowedVertex.begin(); v1 != allowedVertex.begin() + n; ++v1) {
        for(auto v2 = allowedVertex.begin(); v2 != allowedVertex.begin() + n; ++v2) {
            if(g[*v1][*v2]) {
                v[*v1][*v2] = 1; v[*v2][*v1] = 1;
            }
        }
    }

    return v;
}

/**
 * @brief Create a subgraph from an already existing graph
 *        it will take the nodes in the variable allowedVertex
 * 
 * @param g 
 * @param allowedVertex 
 * @return vector<vector<int>> 
 */
vector<vector<int>> createGraph(vector<vector<int>> g, vector<int> allowedVertex) {
    vector<vector<int>> v(g.size(), vector<int>(g.size(), 0));
    for(auto v1 : allowedVertex) {
        for(auto v2 : allowedVertex) {
            if(g[v1][v2]) {
                v[v1][v2] = 1; v[v2][v1] = 1;
            }
        }
    }

    return v;
}

/**
 * @brief find and print (if exists) the n-clique in the given graph (g)
 * 
 * @param g 
 * @param n 
 * @param dir 
 * @return true 
 * @return false 
 */
bool search(const vector<vector<int>>& g, size_t n, string dir = "searchGraph.txt") {

    size_t res = 0;
    vector<int> allowedVertex(g.size());
    std::iota(allowedVertex.begin(), allowedVertex.end(), 0); // range of numbers from 0 to g.size() - 1
    // for(int i = 0; i < g.size(); ++i) allowedVertex.push_back(i);
    vector<int> last(n, 0);

    while(next_permutation(allowedVertex.begin(), allowedVertex.end())) {
    
        // as we are generating all permutations of length g.size()
        // if it is greater than n, we will have repeated permutations of the size n,
        // so we will avoid that using the following two lines
        if(std::equal(allowedVertex.begin(), allowedVertex.begin()+n, last.begin()))
            continue;

        last.clear();
        copy(allowedVertex.begin(), allowedVertex.begin()+n, back_inserter(last)); 

        vector<vector<int>> g2 = createGraph(g, allowedVertex, n);
        graphTOCNF(g2, n);
        res = system("picosat output.cnf > /dev/null");
        if(res == 2560) { 
            translate(g2, dir);
            return true;
        }
    }

    return false;
}

/**
 * @brief find and print (if exists) the n-clique in the given graph (g)
 * 
 * @param g 
 * @param n 
 * @param dir 
 * @return true 
 * @return false 
 */
bool search2(const vector<vector<int>>& g, size_t n, string dir = "searchGraph.txt") {    
    // try one node then get result from picosat, if it is still a k-clique then remove the node, if not, keep it
    size_t res;
    vector<int> allowedVertex(g.size());
    std::iota(allowedVertex.begin(), allowedVertex.end(), 0); // range of numbers from 0 to g.size() - 1

    graphTOCNF(g, n);
    res = system("picosat output.cnf > /dev/null");
    if(res != 2560) return false;
    
    int v = 0;

    while(allowedVertex.size() > 4) {
        vector<int> arr(allowedVertex);
        arr.erase(arr.begin()+v);
        
        vector<vector<int>> g2 = createGraph(g, arr);
        
        graphTOCNF(g2, n);
        
        res = system("picosat output.cnf > /dev/null");

        if(res != 2560) // if it is not a n-clique then do not remove the vertex v
            v = (v+1)%g.size();
        else // it is still a n-clique then remove the vertex
            allowedVertex.erase(allowedVertex.begin()+v);    
    }

    vector<vector<int>> g2 = createGraph(g, allowedVertex);
    graphTOCNF(g2, n);
    translate(g2, dir);

    return true;
}

/**
 * @brief find the highest clique in the vertex
 * 
 * @param g 
 * @return size_t 
 */
size_t optimisation(const vector<vector<int>>& g) {
    size_t min = 2, max = g.size(), p, res;
    
    while(true) {
        if(max - min == 0) break;
        
        p = (max + min) / 2;
        graphTOCNF(g, p);
        res = system("picosat output.cnf > /dev/null");
        
        if(res == 2560)
            min = p; // set lower bound
        else
            max = p-1; // set upper bound
    }
    
    // this line is to find and print the graph in the optGraph.txt file
    bool not_used = search(g, min, "optGraph.txt");

    return min;
}

#endif
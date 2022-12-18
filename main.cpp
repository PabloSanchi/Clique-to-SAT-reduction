#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "reduction.hpp"
#include "translator.hpp"
#include "tools.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

using namespace std;

int help() {
    int op;
    cout << "MENU:" << endl;
    cout << "1. - Search" << endl << "2. - Optimisation" << endl << "> ";
    cin >> op;
    cout << endl;
    return op;
}

int main(void) {
    
    vector< vector<int> > g;
    
    g = fileToGraph("exercise.txt");

    while(true) {
        int op = help();
        if(op == 1) {
            int k;
            cout << "Enter the clique number > ";
            cin >> k;
            
            // int res = search(g, k);
            int res = search2(g, k);
            
            if(res) cout << GREEN;
            else cout << RED;

            cout << "There is " << (res ? "a " : "no ") << "clique k=" << k << RESET << endl;
            if(res) cout << "Check the output file searchGraph.txt" << endl;
            
        } else if(op == 2) {
            cout << BLUE << "Best k=" << optimisation(g) << RESET << endl;
            cout << "Check the output file optGraph.txt" << endl;
        } else {
            cout << "Bye" << endl;
            return 0;
        }
        cout << endl;
    }

    return 0;
}
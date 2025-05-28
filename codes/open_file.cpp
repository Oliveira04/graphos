#include <iostream>
#include <fstream>
#include <string>     
#include "open_file.h" //  para conectar com as declarações

using namespace std;



void processLine(){
    ifstream g2_file("../g2.txt");

    if(!g2_file.is_open()){
        cerr << "Error: Could not open g2.txt" << endl;
        return;
    }
    std::string line;
    while (getline(g2_file, line)) {
        cout << line << endl;
    }
    g2_file.close();
    
}


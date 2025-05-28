#include <iostream>
#include <fstream>
using namespace std;

int main() {
    std::ifstream g2_file("../g2.txt");

    if(!g2_file.is_open()){
        cerr << "Error: Could not open g2.txt" << endl;
        return 1;
    }
    std::string line;
    while (getline(g2_file, line)) {
        cout << line << endl;
    }
    g2_file.close();
    return 0;
}
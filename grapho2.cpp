#include <iostream>
#include <fstream>

int main() {
    std::ifstream g2_file("../g2.txt");

    if(!g2_file.is_open()){
        std::cerr << "Error: Could not open g2.txt" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(g2_file, line)) {
        std::cout << line << std::endl;
    }
    g2_file.close();
    return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // Open file
    std::string filename = "parabola_data.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening \'" << filename << "\'";
    } else {
        std::cout << "Opened \'" << filename << "\'";
    }

    // Skip header
    std::string line;
    std::getline(file, line);

    // Read file into array
    std::vector<std::pair<float, float>> data; // <x, y>
    while (std::getline(file, line, ',')) {
        std::pair<float, float> point;                  // Create point
        std::istringstream stringstream(line);          // Create stream from line
        stringstream >> point.first >> point.second;    // Add x, y values to pair
        data.push_back(point);                          // Add point to data structure
    }

    // Close file
    file.close();

    // Check
    for (const auto &pair : data) {
        std::cout << pair.first << ', ' << pair.second << std::endl;
    }

    return 0;
}
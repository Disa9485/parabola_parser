#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

int main() {
    // Open file
    std::string filename = "parabola_data.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening \'" << filename << "\'";
    } else {
        std::cout << "Opened \'" << filename << "\'";
    }

    /*
    Data structure:
    x,y
    -30.0000,1699.5677
    -29.8999,1693.4813
    -29.7998,1687.6761
    -29.6997,1684.1360
    -29.5997,1675.6774
    -29.4996,1665.6081
    -29.3995,1665.1472
    */

    // Skip header
    std::string line;
    std::getline(file, line);

    // Read file into array
    std::vector<std::pair<float, float>> data; // <x, y>
    while (std::getline(file, line)) {
        // Seperate comma from line
        std::string x, y;
        std::istringstream stringstream(line);
        std::getline(stringstream, x, ',');
        std::getline(stringstream, y, ',');
        
        // Add point to data structure
        data.push_back({std::stof(x), std::stof(y)});         
        
        // std::cout << "X: " << x;
        // std::cout << " Y: " << y << std::endl;
    }

    // Close file
    file.close();

    // // Check data
    // for (const auto &pair : data) {
    //     std::cout << pair.first << ', ' << pair.second << std::endl;
    // }

    // Find min/max x/y values
    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    for (const auto &pair : data) {
        if (pair.first > max_x) max_x = pair.first;
        if (pair.first < min_x) min_x = pair.first;
        if (pair.second > max_y) max_y = pair.second;
        if (pair.second < min_y) min_y = pair.second;
    }


    // Print results
    std::cout << "Max X: " << max_x << std::endl;
    std::cout << "Min X: " << min_x << std::endl;
    std::cout << "Max Y: " << max_y << std::endl;
    std::cout << "Min Y: " << min_y << std::endl;

    return 0;
}
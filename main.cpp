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

    // Find x/y statistics
    float max_x = std::numeric_limits<float>::lowest();
    float max_y = std::numeric_limits<float>::lowest();
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();
    
    float sum_x = 0.0f, sum_y = 0.0f;
    float sum_sq_x = 0.0f, sum_sq_y = 0.0f;
    
    std::vector<float> x_values, y_values;

    for (const auto &pair : data) {
        float x = pair.first;
        float y = pair.second;
        
        // 1. Determine min/max x/y values
        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;
        if (y > max_y) max_y = y;
        if (y < min_y) min_y = y;

        // Sum x/y values
        sum_x += x;
        sum_y += y;

        // Sum squared x/y values
        sum_sq_x += x * x;
        sum_sq_y += y * y;

        x_values.push_back(x);
        y_values.push_back(y);
    }

    // 2. Get x/y mean
    size_t n = data.size();
    float mean_x = sum_x / n;
    float mean_y = sum_y / n;

    // 3. Get x/y standard deviation
    // sum_sq = sum(x^2)
    // variance = (sum_sq / size(x)) - mean(x)^2
    // standard deviation = sqrt(variance)
    float sd_x = std::sqrt((sum_sq_x / n) - (mean_x * mean_x));
    float sd_y = std::sqrt((sum_sq_y / n) - (mean_y * mean_y));

    // 4. Get x/y median
    float med_x, med_y;
    std::sort(x_values.begin(), x_values.end());
    std::sort(y_values.begin(), y_values.end());
    
    // If size of list is even, find mean between two middle indices
    if(n % 2 == 0) {
        med_x = (x_values[n / 2 - 1] + x_values[n / 2]) / 2.0f;
        med_y = (y_values[n / 2 - 1] + y_values[n / 2]) / 2.0f;

    // If size of list is odd, get middle index
    } else {
        med_x = x_values[n / 2];
        med_y = y_values[n / 2];
    }

    // Print statistics results
    std::cout << "Max X: " << max_x << std::endl;
    std::cout << "Min X: " << min_x << std::endl;
    std::cout << "Mean X: " << mean_x << std::endl;
    std::cout << "Med X: " << med_x << std::endl;
    std::cout << "SD X: " << sd_x << std::endl;
    
    std::cout << "Max Y: " << max_y << std::endl;
    std::cout << "Min Y: " << min_y << std::endl;
    std::cout << "Mean Y: " << mean_y << std::endl;
    std::cout << "Med Y: " << med_y << std::endl;
    std::cout << "SD Y: " << sd_y << std::endl;
    

    return 0;
}
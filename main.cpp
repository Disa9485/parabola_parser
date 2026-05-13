#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

#include <Eigen/Dense>

void stats(std::vector<std::pair<float, float>> &data) {
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
    
    std::vector<float> x_values, y_values; // Save for median
    std::pair<float, float> max_y_point, min_y_point; // Save for parabola vertex

    // Iterate through each point
    for (const auto &point : data) {
        float x = point.first;
        float y = point.second;
        
        // 1. Determine min/max x/y values
        if (x > max_x) max_x = x;
        if (x < min_x) min_x = x;
        if (y > max_y) { max_y = y; max_y_point = point; }
        if (y < min_y) { min_y = y; min_y_point = point; }

        // Sum x/y values
        sum_x += x;
        sum_y += y;

        // Sum squared x/y values
        sum_sq_x += x * x;
        sum_sq_y += y * y;

        // Store x/y values
        x_values.push_back(x);
        y_values.push_back(y);
    }

    // 2. Get x/y mean
    size_t n = data.size();
    float mean_x = sum_x / n;
    float mean_y = sum_y / n;

    // 3. Get x/y standard deviation (population type; do n - 1 for sample type)
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
    std::cout << "\n#### STATISTICS ####" << std::endl;

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

    // Get parabola vertex
    bool opens_up = med_y < data.front().second && med_y < data.back().second;
    bool opens_down = med_y > data.front().second && med_y > data.back().second;
    if (opens_up) {
        std::cout << "Parabola opens upward. Vertex: [" << min_y_point.first << "," << min_y_point.second << "]" << std::endl;
    } else if (opens_down) {
        std::cout << "Parabola opens downward. Vertex: [" << max_y_point.first << "," << max_y_point.second << "]" << std::endl;
    } else {
        std::cout << "Could not find parabola direction." << std::endl;
    }
}

void fit(std::vector<std::pair<float, float>> &data) {
    // I chose to implement linear regression with eigen since it seemed to be one of the best C++ libraries for efficient linear algebra.
    // https://codepal.ai/code-generator/query/QvutKoAV/c-code-for-linear-regression-using-eigen
    
    // Fit to:          y = ax² + bx + c
    // Then convert:    y = a(x − h)² + k (vertex form)
    //                  y = a(x² − 2hx + h²) + k
    //                  y = ax² − 2ahx + ah² + k

    // Where:
    // b = -2ah
    // c = ah² + k

    const int n = static_cast<int>(data.size());
    if (n < 3) {
        std::cerr << "Need at least 3 points to fit parabola." << std::endl;
        return;
    }

    // Design matrix:
    // [ x0² x0  1 ]
    // [ x1² x1  1 ]
    // [ x2² x2  1 ]
    //
    // Coefficient vector:
    // [ a ]
    // [ b ]
    // [ c ]
    //
    // Solve:
    // X * coeffs ≈ Y

    // Populate matrices
    Eigen::MatrixXd X(n, 3);
    Eigen::VectorXd Y(n);
    for (int i = 0; i < n; i++) {
        double x = data[i].first;
        double y = data[i].second;

        // Design matrix
        X(i, 0) = x * x;
        X(i, 1) = x;
        X(i, 2) = 1.0;

        Y(i) = y;
    }

    // Solve least-squares system using QR decomposition
    Eigen::Vector3d coeffs = X.colPivHouseholderQr().solve(Y);
    double a = coeffs(0);
    double b = coeffs(1);
    double c = coeffs(2);

    // Prevent divide-by-zero if parabola degenerates to line
    if (std::abs(a) < 1e-12) {
        std::cerr << "Parabola is too linear to fit." << std::endl;
        return;
    }

    // Convert standard form to vertex form
    double h = -b / (2.0f * a);          // b = -2ah     so h = -b/2a
    double k = c - (b * b) / (4.0f * a); // c = ah² + k  so k = c-b²/4a

    // Calculate RMSE (root mean squared error)
    // https://statisticsbyjim.com/regression/root-mean-square-error-rmse/
    double squared_error_sum = 0.0f;
    for (const auto& point : data) {
        double x = point.first;
        double y = point.second;

        double predicted_y = a * (x - h) * (x - h) + k;
        double error = y - predicted_y;
        squared_error_sum += error * error;
    }
    double rmse = std::sqrt(squared_error_sum / data.size());

    // Print results
    std::cout << "\n#### CURVE FIT ####" << std::endl;
    std::cout << "Equation: y = " << a << "(x - " << h << ")^2 + " << k << std::endl;
    std::cout << "a: " << a << std::endl;
    std::cout << "h: " << h << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "RMSE: " << rmse << std::endl;
}

void plot(std::vector<std::pair<float, float>> &data) {

}

int main(int argc, char* argv[]) {
    // Get filename
    if (argc < 2) {
        std::cerr << "Usage: ./parabola_parser <filename> [--stats] [--fit] [--plot]" << std::endl;
        return 1;
    }

    // Open file
    std::string filename = argv[1]; // "parabola_data.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening \'" << filename << "\'" << std::endl;
        return 1;
    } else {
        std::cout << "Opened \'" << filename << "\'" << std::endl;
    }

    // Parse argument flags
    bool show_stats = false, show_fit = false, show_plot = false;
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i]; // Get argument
        if (arg == "--stats") show_stats = true;
        else if (arg == "--fit") show_fit = true;
        else if (arg == "--plot") show_plot = true;
        else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
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
    // Detect outliers, an outlier is any row with data that is not in form '<float>,<float>'.
    size_t line_n = 1;
    std::vector<std::pair<float, float>> data; // <x, y>
    while (std::getline(file, line)) {
        line_n++;
        if(line.empty()) {
            std::cerr << "Line " << line_n << " is empty. Skipping." << std::endl;
            continue;
        }

        // Seperate comma from line
        std::string x, y, _;
        std::istringstream stringstream(line);

        // Check for missing values
        if(!std::getline(stringstream, x, ',') ||
           !std::getline(stringstream, y, ',')) {
            std::cerr << "Line " << line_n << " is missing x or y value. Skipping." << std::endl;
            continue;
        }

        // Check for too many values
        if(std::getline(stringstream, _, ',')) {
            std::cerr << "Line " << line_n << " has too many values. Skipping." << std::endl;
            continue;
        }
        
        // Add point to data structure
        try {
            data.push_back({std::stof(x), std::stof(y)});   
        } catch (const std::invalid_argument&) {
            std::cerr << "Line " << line_n << " has a NaN value. Skipping." << std::endl;
            continue;
        } catch (const std::out_of_range&) {
            std::cerr << "Line " << line_n << " is out of range. Skipping." << std::endl;
            continue;
        }

        // std::cout << "X: " << x;
        // std::cout << " Y: " << y << std::endl;
    }

    // Close file
    file.close();

    // Check for data
    if (data.empty()) {
        std::cerr << "No data loaded.\n";
        return 1;
    }

    // // Check data
    // for (const auto &pair : data) {
    //     std::cout << pair.first << ', ' << pair.second << std::endl;
    // }

    // Show operations
    if (show_stats) stats(data);
    if (show_fit) fit(data);
    if (show_plot) plot(data);

    return 0;
}
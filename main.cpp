#include "operations.hpp"

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
    bool show_stats = false, show_fit = false, show_plot = false, show_mirror = false;
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i]; // Get argument
        if (arg == "--stats") show_stats = true;
        else if (arg == "--fit") show_fit = true;
        else if (arg == "--plot") show_plot = true;
        else if (arg == "--mirror") show_mirror = true;
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

    // Parse data
    std::vector<std::pair<float, float>> data; // <x, y>
    std::ostringstream buffer; buffer << file.rdbuf();
    std::string raw_data = buffer.str();
    parse(raw_data, data);

    // Close file
    file.close();

    // Check for data
    if (data.empty()) {
        std::cerr << "No data loaded." << std::endl;
        return 1;
    }

    // // Check data
    // for (const auto &pair : data) {
    //     std::cout << pair.first << ', ' << pair.second << std::endl;
    // }

    // Show operations
    StatsData stats_data = {};
    FitData fit_data = {};
    if (show_stats) stats(data, stats_data);
    if (show_fit) fit(data, fit_data);
    if (show_mirror) mirror(data, fit_data);
    if (show_plot) plot(data, stats_data, fit_data);

    /*
    Performance summary:
    n = number of data points

    Timings at 1200 data points
    Parse Time: 933 us
    Stats Time:      2023 us
    Fit Time:        1077 us
    Mirror Time:     1669 us
    Plot Setup Time: 106612 us

    Big O
    File parse:                 O(n)
    Stats (min, max, mean, sd): O(n)
    Stats (med):                O(n log n)
    Fit:                        O(n)
    Mirror:                     O(n)
    Plot setup:                 O(n)
    Plot render:                O(n) per frame

    Stats (med) will be much slower at 12000000 data points but still usable
    Plot render will likely be unusable at 12000000 data points

    If we had 12000000 data points instead of 1200, we'd need to change
    plot so we only render representative points instead of all of them.
    */

    return 0;
}
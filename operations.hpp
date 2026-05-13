#include <Eigen/Dense>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <chrono>

struct StatsData {
    float max_x = std::numeric_limits<float>::lowest();
    float min_x = std::numeric_limits<float>::max();
    float mean_x = 0.0f;
    float med_x = 0.0f;
    float sd_x = 0.0f;

    float max_y = std::numeric_limits<float>::lowest();
    float min_y = std::numeric_limits<float>::max();
    float mean_y = 0.0f;
    float med_y = 0.0f;
    float sd_y = 0.0f;

    bool opens_up = false;
    std::pair<float, float> up_parabola_vertex, down_parabola_vertex; // Parabola vertex
};

struct FitData {
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double h = 0.0;
    double k = 0.0;
    double rmse = 0.0;
    bool valid = false;
};

void parse(const std::string& raw_data, std::vector<std::pair<float, float>>& data);
void stats(const std::vector<std::pair<float, float>>& data, StatsData& stats_data);
void fit(const std::vector<std::pair<float, float>> &data, FitData &fit_data);
void mirror(const std::vector<std::pair<float, float>> &data, FitData &fit_data);
void plot(const std::vector<std::pair<float, float>>& data, const StatsData &stats_data, const FitData& fit_data);

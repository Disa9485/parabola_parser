#include "operations.hpp"

void parse(const std::string& raw_data, std::vector<std::pair<float, float>>& data) {
    auto start = std::chrono::high_resolution_clock::now(); // Parse start

    // Create datastream
    std::istringstream datastream(raw_data);

    // Skip header
    std::string line;
    std::getline(datastream, line);

    // Read datastream into vector
    // Detect outliers, an outlier is any row with data that is not in form '<float>,<float>'.
    size_t line_n = 1;
    while (std::getline(datastream, line)) {
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

    auto end = std::chrono::high_resolution_clock::now(); // Parse end
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Parse Time: " << us << " us" << std::endl;
}

void stats(const std::vector<std::pair<float, float>>& data, StatsData& stats_data) {
    auto start = std::chrono::high_resolution_clock::now(); // Stats start

    // x/y sums
    float sum_x = 0.0f, sum_y = 0.0f;
    float sum_sq_x = 0.0f, sum_sq_y = 0.0f;

    std::vector<float> x_data, y_data; // Save for median

    // Iterate through each point
    for (const auto &point : data) {
        float x = point.first;
        float y = point.second;
        
        // 1. Determine min/max x/y values
        if (x > stats_data.max_x) stats_data.max_x = x;
        if (x < stats_data.min_x) stats_data.min_x = x;
        if (y > stats_data.max_y) { stats_data.max_y = y; stats_data.up_parabola_vertex = point; }
        if (y < stats_data.min_y) { stats_data.min_y = y; stats_data.down_parabola_vertex = point; }

        // Sum x/y values
        sum_x += x;
        sum_y += y;

        // Sum squared x/y values
        sum_sq_x += x * x;
        sum_sq_y += y * y;

        // Store x/y values
        x_data.push_back(x);
        y_data.push_back(y);
    }

    // 2. Get x/y mean
    size_t n = data.size();
    stats_data.mean_x = sum_x / n;
    stats_data.mean_y = sum_y / n;

    // 3. Get x/y standard deviation (population type; do n - 1 for sample type)
    // sum_sq = sum(x^2)
    // variance = (sum_sq / size(x)) - mean(x)^2
    // standard deviation = sqrt(variance)
    stats_data.sd_x = std::sqrt((sum_sq_x / n) - (stats_data.mean_x * stats_data.mean_x));
    stats_data.sd_y = std::sqrt((sum_sq_y / n) - (stats_data.mean_y * stats_data.mean_y));

    // 4. Get x/y median
    std::sort(x_data.begin(), x_data.end());
    std::sort(y_data.begin(), y_data.end());
    
    // If size of list is even, find mean between two middle indices
    if(n % 2 == 0) {
        stats_data.med_x = (x_data[n / 2 - 1] + x_data[n / 2]) / 2.0f;
        stats_data.med_y = (y_data[n / 2 - 1] + y_data[n / 2]) / 2.0f;

    // If size of list is odd, get middle index
    } else {
        stats_data.med_x = x_data[n / 2];
        stats_data.med_y = y_data[n / 2];
    }

    // Print statistics results
    std::cout << "\n#### STATISTICS ####" << std::endl;

    std::cout << "Max X: " << stats_data.max_x << std::endl;
    std::cout << "Min X: " << stats_data.min_x << std::endl;
    std::cout << "Mean X: " << stats_data.mean_x << std::endl;
    std::cout << "Med X: " << stats_data.med_x << std::endl;
    std::cout << "SD X: " << stats_data.sd_x << std::endl;
    
    std::cout << "Max Y: " << stats_data.max_y << std::endl;
    std::cout << "Min Y: " << stats_data.min_y << std::endl;
    std::cout << "Mean Y: " << stats_data.mean_y << std::endl;
    std::cout << "Med Y: " << stats_data.med_y << std::endl;
    std::cout << "SD Y: " << stats_data.sd_y << std::endl;

    // Get parabola vertex
    bool opens_up = stats_data.med_y < data.front().second && stats_data.med_y < data.back().second;
    bool opens_down = stats_data.med_y > data.front().second && stats_data.med_y > data.back().second;
    if (opens_up) {
        std::cout << "Parabola opens upward. Vertex Approx: [" << stats_data.down_parabola_vertex.first << "," << stats_data.down_parabola_vertex.second << "]" << std::endl;
    } else if (opens_down) {
        std::cout << "Parabola opens downward. Vertex Approx: [" << stats_data.up_parabola_vertex.first << "," << stats_data.up_parabola_vertex.second << "]" << std::endl;
    } else {
        std::cout << "Could not find parabola direction." << std::endl;
    }

    std::cout << "\n#### PARSE ####" << std::endl;

    auto end = std::chrono::high_resolution_clock::now(); // Stats end
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Stats Time: " << us << " us" << std::endl;
}

void fit(const std::vector<std::pair<float, float>> &data, FitData &fit_data) {
    auto start = std::chrono::high_resolution_clock::now(); // Fit start
    
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
    fit_data.a = coeffs(0);
    fit_data.b = coeffs(1);
    fit_data.c = coeffs(2);

    // Prevent divide-by-zero if parabola degenerates to line
    if (std::abs(fit_data.a) < 1e-12) {
        std::cerr << "Parabola is too linear to fit." << std::endl;
        return;
    }

    // Convert standard form to vertex form
    fit_data.h = -fit_data.b / (2.0f * fit_data.a);          // b = -2ah     so h = -b/2a
    fit_data.k = fit_data.c - (fit_data.b * fit_data.b) / (4.0f * fit_data.a); // c = ah² + k  so k = c-b²/4a

    // Calculate RMSE (root mean squared error)
    // https://statisticsbyjim.com/regression/root-mean-square-error-rmse/
    double squared_error = 0.0f;
    for (const auto& point : data) {
        double x = point.first;
        double y = point.second;

        double predicted_y = fit_data.a * (x - fit_data.h) * (x - fit_data.h) + fit_data.k;
        double error = y - predicted_y;
        squared_error += error * error;
    }
    fit_data.rmse = std::sqrt(squared_error / data.size());

    fit_data.valid = true;

    // Print results
    std::cout << "\n#### CURVE FIT ####" << std::endl;
    std::cout << "Equation: y = " << fit_data.a << "(x - " << fit_data.h << ")^2 + " << fit_data.k << std::endl;
    std::cout << "a: " << fit_data.a << std::endl;
    std::cout << "h: " << fit_data.h << std::endl;
    std::cout << "k: " << fit_data.k << std::endl;
    std::cout << "RMSE: " << fit_data.rmse << std::endl;

    auto end = std::chrono::high_resolution_clock::now(); // Fit end
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Fit Time: " << us << " us" << std::endl;
}

void mirror(const std::vector<std::pair<float, float>> &data, FitData &fit_data) {
    auto start = std::chrono::high_resolution_clock::now(); // Mirror start
    
    // Check data
    if (data.empty()) {
        std::cerr << "No data, cannot check mirror." << std::endl;
        return;
    }

    // Check fit data
    if (!fit_data.valid) {
        std::cerr << "No fit data, cannot check mirror." << std::endl;
        return;
    }

    // Iterate through each point and compare to fitted line
    size_t lh_size = 0, rh_size = 0;
    double lh_squared_error = 0.0, rh_squared_error = 0.0;
    double max_lh_error = 0.0, max_rh_error = 0.0;
    for (const auto& point : data) {
        double x = point.first;
        double y = point.second;

        // Get fit y
        double predicted_y = fit_data.a * (x - fit_data.h) * (x - fit_data.h) + fit_data.k;
        double error = y - predicted_y;
        double abs_error = std::abs(error);

        // Check left side y (h = vertex x)
        if (x < fit_data.h) {
            lh_size++;
            lh_squared_error += error * error;
            max_lh_error = std::max(max_lh_error, abs_error);
        
        // Check right side y (h = vertex x)
        } else if (x > fit_data.h) {
            rh_size++;
            rh_squared_error += error * error;
            max_rh_error = std::max(max_rh_error, abs_error);
        }
    }

    // Get left/right half rmse and difference
    double lh_rmse = std::sqrt(lh_squared_error / lh_size);
    double rh_rmse = std::sqrt(rh_squared_error / rh_size);
    double rmse_difference = std::abs(lh_rmse - rh_rmse);
    double rmse_relative_difference = rmse_difference / std::max(std::max(lh_rmse, rh_rmse), 1e-12); 

    // Print mirror results
    std::cout << "\n#### MIRROR ####" << std::endl;
    std::cout << "Size - LH: " << lh_size << ", RH: " << rh_size << std::endl;
    std::cout << "RMSE - LH: " << lh_rmse << ", RH: " << rh_rmse << std::endl;
    std::cout << "RMSE - difference: " << rmse_difference << ", relative difference: " << rmse_relative_difference << std::endl;

    // If RMSE relative difference is too large, not approximately symmetric
    if (rmse_relative_difference <= 0.05) {
        std::cout << "Result: left and right halves are approx. symmetric." << std::endl;
    } else {
        std::cout << "Result: left and right halves are not symmetric." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now(); // Mirror end
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Mirror Time: " << us << " us" << std::endl;
}

void plot(const std::vector<std::pair<float, float>>& data, const StatsData &stats_data, const FitData& fit_data) {
    auto start = std::chrono::high_resolution_clock::now(); // Plot setup start
    
    // Check data
    if (data.empty()) {
        std::cerr << "No data, cannot plot.\n";
        return;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return;
    }

    // Create Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Plot", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Split data in x/y vectors
    std::vector<float> x_data, y_data;
    for (const auto& point : data) {
        x_data.push_back(point.first);
        y_data.push_back(point.second);
    }
    
    // Get fit x/y data
    std::vector<double> x_fit_data, y_fit_data;
    size_t fit_points = 500; // Number of points for fitted line
    if (fit_data.valid) {
        for (size_t i = 0; i < fit_points; i++) {
            // Get current normalized position
            double t = static_cast<double>(i) / static_cast<double>(fit_points - 1);

            // Get interpolated x value along interval
            double x = stats_data.min_x + t * (stats_data.max_x - stats_data.min_x);

            // Plug x value into parabola equation (y = a(x - h)^2 + k)
            double y = fit_data.a * (x - fit_data.h) * (x - fit_data.h) + fit_data.k;

            x_fit_data.push_back(x);
            y_fit_data.push_back(y);
        }
    }

    std::cout << "\n#### PLOT ####" << std::endl;

    auto end = std::chrono::high_resolution_clock::now(); // Plot setup end
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Plot Setup Time: " << us << " us" << std::endl;

    start = std::chrono::high_resolution_clock::now(); // Plot render start

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Plot data with ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Parabola Data");
        if (ImPlot::BeginPlot("Parabola Data")) {
            ImPlot::SetupAxes("X", "Y"); // Setup axes

            // Scatter plot parabola data
            ImPlot::PlotScatter(
                "Data",
                x_data.data(),
                y_data.data(),
                x_data.size()
            );

            // Line plot fit data
            if (fit_data.valid) {
                ImPlot::PlotLine(
                    "Fitted Data",
                    x_fit_data.data(),
                    y_fit_data.data(),
                    x_fit_data.size()
                );
            }

            ImPlot::EndPlot(); // End plot
        }
        ImGui::End();
        ImGui::Render();

        // Clear framebuffer
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw framebuffer
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap framebuffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    end = std::chrono::high_resolution_clock::now(); // Plot render end
    us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Plot Render Time: " << us << " us" << std::endl;
}

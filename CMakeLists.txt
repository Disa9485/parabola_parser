cmake_minimum_required(VERSION 3.20)

project(parabola_parser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)

add_library(parabola_operations
    operations.cpp
)

target_include_directories(parabola_operations
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parabola_operations
    PUBLIC
    Eigen3::Eigen
    glfw
    glad::glad
    imgui::imgui
    implot::implot
)

add_executable(parabola_parser
    main.cpp
)

target_include_directories(parabola_parser PRIVATE
    .
)

target_link_libraries(parabola_parser
    PRIVATE
    parabola_operations
)

add_executable(parabola_tests
    tests.cpp
)

target_link_libraries(parabola_tests
    PRIVATE
    parabola_operations
    doctest::doctest
)

add_test(NAME parabola_tests COMMAND parabola_tests)

add_custom_command(TARGET parabola_parser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/parabola_data.txt"
        $<TARGET_FILE_DIR:parabola_parser>/parabola_data.txt
)
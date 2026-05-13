cmake_minimum_required(VERSION 3.20)

project(parabola_parser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 CONFIG REQUIRED)

add_executable(parabola_parser
    main.cpp
)

target_link_libraries(parabola_parser
    PRIVATE Eigen3::Eigen
)

target_include_directories(parabola_parser PRIVATE
    .
)

add_custom_command(TARGET parabola_parser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/parabola_data.txt"
        $<TARGET_FILE_DIR:parabola_parser>/parabola_data.txt
)
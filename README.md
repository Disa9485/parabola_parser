### Description
parabola_parser reads csv parabola data and optionally computes statistics, fits a curve, checks parabola symmetry, or displays a plot.

### Requirements
- C++20 Compiler
- CMake 3.20+
- vcpkg
- OpenGL compatible system

Tested On
- Windows 11
- Visual Studio 2022 MSVC
- CMake 3.20+
- vcpkg manifest mode
- C++20

### Libraries
- Eigen3
- GLFW
- GLAD
- Dear ImGui
- ImPlot
- doctest

### Fresh build:
Make sure `CMakePresets.json` points to your local `vcpkg.cmake`.

From the project root, build and run with:
```bat
cmake --preset default --fresh
cmake --build --preset release
.\build\Release\parabola_parser.exe parabola_data.txt --stats --fit --mirror --plot
```

To run tests:
```
ctest --test-dir build -C Release --output-on-failure
OR
.\build\Release\parabola_tests.exe -s
```

If dependencies are missing, run:
where vcpkg
<vcpkg-root>\vcpkg.exe install
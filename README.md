### Description

### Fresh build:
Make sure `CMakePresets.json` points to your local `vcpkg.cmake`.

From the project root, build and run with:
```bat
cmake --preset default --fresh
cmake --build --preset release
.\build\Release\parabola_parser.exe parabola_data.txt --stats --fit --plot
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
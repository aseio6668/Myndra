@echo off
REM Pomegranate Build Script for Windows
REM Builds the Pomegranate programming language compiler and runtime

echo Building Pomegranate Programming Language...

REM Create build directory
if not exist "build" mkdir build

cd build

REM Configure with CMake
echo Configuring build system...
cmake .. -DCMAKE_BUILD_TYPE=Release

REM Build the project
echo Compiling...
cmake --build . --config Release --parallel

echo Build completed successfully!
echo.
echo Executables created:
echo   - pomegranate.exe (main compiler/interpreter)
echo   - pom-pkg.exe (package manager utility)
echo.
echo To run examples:
echo   pomegranate.exe ..\examples\hello_world.pom --run
echo   pomegranate.exe --interactive

pause

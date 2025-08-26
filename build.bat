@echo off
REM Myndra Build Script for Windows
REM Builds the Myndra programming language compiler and runtime

echo Building Myndra Programming Language...

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
echo   - myndra.exe (main compiler/interpreter)
echo   - myn-pkg.exe (package manager utility)
echo.
echo To run examples:
echo   myndra.exe ..\examples\hello_world.myn --run
echo   myndra.exe --interactive

pause

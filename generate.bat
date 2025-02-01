@echo off
:: Set up variables for build directory and generator
set BUILD_DIR=build
set GENERATOR="Visual Studio 17 2022"

:: Run CMake to generate project files
echo Generating project files...
cmake -S . -B %BUILD_DIR% -G %GENERATOR%

:: Check if CMake succeeded
if %errorlevel% neq 0 (
    echo CMake generation failed!
    exit /b %errorlevel%
)

:: Notify the user
echo Project files generated successfully in %BUILD_DIR%.

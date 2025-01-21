@echo off
:: Set up variables for build directory and generator
set BUILD_DIR=build
set GENERATOR="Visual Studio 17 2022"

:: Remove previous build directory if it exists
if exist %BUILD_DIR% (
    echo Removing existing build directory...
    rmdir /s /q %BUILD_DIR%
)

:: Create a new build directory
echo Creating build directory...
mkdir %BUILD_DIR%

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
pause

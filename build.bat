@echo off

REM Define variables
set BUILD_DIR=build
set GENERATOR="Visual Studio 17 2022"

REM Clear the console
cls

echo === Building the Project ===

REM Check if the build directory exists
if not exist %BUILD_DIR% (
    echo Creating build directory...
    mkdir %BUILD_DIR%
)

REM Navigate to the build directory
cd %BUILD_DIR%

REM Run CMake configuration with -Wno-dev to suppress developer warnings
cmake -G %GENERATOR% -Wno-dev ..
if errorlevel 1 goto :error

REM Build the project with detailed output
cmake --build . --config Release --verbose
if errorlevel 1 goto :error

echo === Build Completed Successfully ===
goto :end

:error
echo === Build Failed ===
exit /b 1

:end
cd ..

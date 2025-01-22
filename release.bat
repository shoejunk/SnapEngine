@echo off

REM Define variables
set BUILD_DIR=build
set EXECUTABLE=SnapEngineApp.exe
set CONFIG=Release

REM Clear the console
cls

echo === Running Release Version ===

REM Check if the build directory exists
if not exist %BUILD_DIR% (
    echo Build directory does not exist. Please build the project first.
    exit /b 1
)

REM Navigate to the release directory
cd %BUILD_DIR%\%CONFIG%

REM Check if the executable exists
if not exist %EXECUTABLE% (
    echo Executable not found. Please build the project first.
    exit /b 1
)

REM Run the executable
%EXECUTABLE%
if errorlevel 1 goto :error

echo === Execution Completed Successfully ===
goto :end

:error
echo === Execution Failed ===
exit /b 1

:end
cd ..\..

@echo off
:: Navigate to the build directory
cd /d %~dp0build

:: Run ctest with Debug configuration
echo Running tests with Debug configuration...
ctest -C Debug -V

:: Pause to allow the user to see the output
pause

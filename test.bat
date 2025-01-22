@echo off
:: test.bat
:: This batch file assumes you've already configured and built the project,
:: and that the "build" folder is next to "test.bat" (i.e., SnapEngine/build).

:: Push the current directory, then move into the build folder
pushd %~dp0build

echo Running tests with Debug configuration...
ctest -C Release -V

:: Pop back to the original directory (root folder)
popd

pause

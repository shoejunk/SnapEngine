@echo off
echo Building SnapEngine...

if not exist "build" mkdir build
cd build

cmake -G "Visual Studio 17 2022" -A x64 ..
if %ERRORLEVEL% neq 0 (
    echo CMake generation failed
    exit /b %ERRORLEVEL%
)

cmake --build . --config Debug
if %ERRORLEVEL% neq 0 (
    echo Build failed
    exit /b %ERRORLEVEL%
)

echo Running tests...
bin\Debug\SnapEngineApp.exe --test
if %ERRORLEVEL% neq 0 (
    echo Tests failed
    exit /b %ERRORLEVEL%
)

echo All tests passed successfully!

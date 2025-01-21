cmake_minimum_required(VERSION 3.20)
project(SnapEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include CPM.cmake (dependency manager)
include(cmake/CPM.cmake)

# Add nlohmann/json package
CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.11.3
  GITHUB_REPOSITORY nlohmann/json
  OPTIONS
    "JSON_BuildTests OFF"
)

# -------------------------------------------------------
# Gather all source/header files in src/
# -------------------------------------------------------
file(GLOB_RECURSE SNAPENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# -------------------------------------------------------
# Create the SnapEngine library
# -------------------------------------------------------
add_library(SnapEngine STATIC ${SNAPENGINE_SOURCES})
target_include_directories(SnapEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# -------------------------------------------------------
# Create the main executable
# -------------------------------------------------------
add_executable(SnapEngineApp main.cpp)
target_link_libraries(SnapEngineApp PRIVATE SnapEngine nlohmann_json)

# -------------------------------------------------------
# Enable Testing with CTest
# -------------------------------------------------------
enable_testing()

# Create a test that calls SnapEngineApp in "--test" mode
# This assumes "main.cpp" checks for "--test" and calls Window::test(), etc.
add_test(
    NAME SnapEngineAllTests
    COMMAND SnapEngineApp --test
)

# By default, CMake places tests in the Debug configuration on Windows
# when building multi-config (e.g., Visual Studio). The "-C Debug" part
# in your .bat file will ensure Debug-mode tests are run.
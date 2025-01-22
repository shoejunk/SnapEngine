cmake_minimum_required(VERSION 3.20)
project(SnapEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include CPM.cmake (dependency manager)
include(cmake/CPM.cmake)

# Add nlohmann/json package via CPM
CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.11.3
  GITHUB_REPOSITORY nlohmann/json
  OPTIONS
    "JSON_BuildTests OFF"   # Disable json's own tests
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

# Ensure SnapEngine can include its own headers and nlohmann/json
target_include_directories(SnapEngine PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:src>"
)

# Link SnapEngine with nlohmann_json (so it can use nlohmann/json in its .cpp files)
target_link_libraries(SnapEngine
    PUBLIC
        nlohmann_json
)

# -------------------------------------------------------
# Create the main executable
# -------------------------------------------------------
add_executable(SnapEngineApp main.cpp)

# Link SnapEngineApp with SnapEngine (and transitively nlohmann_json)
target_link_libraries(SnapEngineApp
    PRIVATE
        SnapEngine
)

# -------------------------------------------------------
# Copy snapengine_data.json after build
# -------------------------------------------------------
add_custom_command(TARGET SnapEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/snapengine_data.json"
        $<TARGET_FILE_DIR:SnapEngineApp>
)

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

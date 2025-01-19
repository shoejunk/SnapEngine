cmake_minimum_required(VERSION 3.20)

# Set the project name and specify it as a C++20 project
project(SnapEngine LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include CPM.cmake for dependency management
include(cmake/CPM.cmake)

# Define the main target for SnapEngine
add_library(SnapEngine STATIC
    src/SnapEngine.cpp
    include/SnapEngine/SnapEngine.hpp
)

# Specify include directories for SnapEngine
target_include_directories(SnapEngine PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add the main.cpp file as part of the executable target
add_executable(SnapEngineApp main.cpp)
target_link_libraries(SnapEngineApp PRIVATE SnapEngine)

# Enable testing (optional)
if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()

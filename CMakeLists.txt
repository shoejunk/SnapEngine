cmake_minimum_required(VERSION 3.20)
project(SnapEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# Force all libraries to be built statically (no .dll)
# -------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries as static" FORCE)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME nlohmann_json
    VERSION 3.11.3
    GITHUB_REPOSITORY nlohmann/json
    OPTIONS
        "JSON_BuildTests=OFF"
)

CPMAddPackage(
    NAME assimp
    GITHUB_REPOSITORY assimp/assimp
    VERSION 5.2.5
    OPTIONS
        "ASSIMP_BUILD_TESTS=OFF"
        "ASSIMP_BUILD_SAMPLES=OFF"
        "ASSIMP_BUILD_DOCS=OFF"
        "BUILD_SHARED_LIBS=OFF"
)

# -------------------------------------------------------------------
# Gather sources & create the SnapEngine static library
# -------------------------------------------------------------------
file(GLOB_RECURSE SNAPENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_library(SnapEngine STATIC ${SNAPENGINE_SOURCES})

target_include_directories(SnapEngine PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:src>"
)

target_link_libraries(SnapEngine
    PUBLIC
        nlohmann_json
        assimp
)

# -------------------------------------------------------------------
# Create the main executable
# -------------------------------------------------------------------
add_executable(SnapEngineApp main.cpp)

target_link_libraries(SnapEngineApp PRIVATE SnapEngine)

# Copy snapengine_data.json after build
add_custom_command(TARGET SnapEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/snapengine_data.json"
        $<TARGET_FILE_DIR:SnapEngineApp>

    # Copy test_assets as well
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test_assets"
        "$<TARGET_FILE_DIR:SnapEngineApp>/test_assets"
)

enable_testing()
add_test(
    NAME SnapEngineAllTests
    COMMAND SnapEngineApp --test
)

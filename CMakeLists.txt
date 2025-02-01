cmake_minimum_required(VERSION 3.15)
project(SnapEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Function to set MSVC-specific options for a target
function(set_msvc_options target)
    if(MSVC)
        get_target_property(target_type ${target} TYPE)
        if(target_type STREQUAL "INTERFACE_LIBRARY")
            target_compile_options(${target} INTERFACE 
                /Zc:__cplusplus 
                /Zc:preprocessor
            )
        else()
            target_compile_options(${target} PRIVATE 
                /Zc:__cplusplus 
                /Zc:preprocessor
            )
        endif()
    endif()
endfunction()

include(cmake/CPM.cmake)

# Add dependencies
CPMAddPackage(
    NAME nlohmann_json
    VERSION 3.11.3
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    OPTIONS 
        "JSON_BuildTests OFF"
)

# Add Assimp (this will bring in ZLIB)
CPMAddPackage(
    NAME assimp
    VERSION 5.2.5
    URL https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.zip
    OPTIONS 
        "ASSIMP_BUILD_TESTS OFF"
        "ASSIMP_INSTALL OFF"
        "BUILD_SHARED_LIBS OFF"
        "ASSIMP_BUILD_ZLIB YES"
)

CPMAddPackage(
    NAME glm
    VERSION 0.9.9.8
    URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
)

# Add GLFW
CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.3.9
    OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_INSTALL OFF"
)

# Add GLEW
CPMAddPackage(
    NAME glew
    GITHUB_REPOSITORY Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0
    OPTIONS
        "glew-cmake_BUILD_SHARED OFF"
        "glew-cmake_BUILD_STATIC ON"
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Gather sources & create the SnapEngine static library
file(GLOB_RECURSE SNAPENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_library(SnapEngine STATIC ${SNAPENGINE_SOURCES})
set_msvc_options(SnapEngine)

target_include_directories(SnapEngine PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:src>"
    ${glm_SOURCE_DIR}
)

target_link_libraries(SnapEngine
    PUBLIC
        nlohmann_json
        assimp
        glm
        glfw
        libglew_static
        OpenGL::GL
)

# Create the main executable
add_executable(SnapEngineApp main.cpp)
set_msvc_options(SnapEngineApp)

target_link_libraries(SnapEngineApp
    PRIVATE
        SnapEngine
)

# Set the working directory for Visual Studio to match the executable's directory
set_target_properties(SnapEngineApp PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SnapEngineApp>"
)

# Copy snapengine_data.json and assets after build
add_custom_command(TARGET SnapEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/snapengine_data.json"
        $<TARGET_FILE_DIR:SnapEngineApp>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test_assets"
        "$<TARGET_FILE_DIR:SnapEngineApp>/test_assets"
)

# Enable Testing
enable_testing()
add_test(
    NAME SnapEngineAllTests
    COMMAND SnapEngineApp --test
)

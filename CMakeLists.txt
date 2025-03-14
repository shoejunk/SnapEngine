cmake_minimum_required(VERSION 3.15)
project(SnapEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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
        "BUILD_SHARED_LIBS ON"
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
        "BUILD_SHARED_LIBS ON"
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

# Source files
set(SOURCES
    src/Window.cpp
    src/WindowManager.cpp
    src/DataManager.cpp
    src/Model.cpp
    src/Scene.cpp
    src/Camera.cpp
    src/Tests.cpp
    src/Shader.cpp
    src/Mesh.cpp
)

# Header files
set(HEADERS
    src/Window.h
    src/WindowManager.h
    src/DataManager.h
    src/Model.h
    src/Mesh.h
    src/Vertex.h
    src/Tests.h
    src/Scene.h
    src/Camera.h
    src/Shader.h
)

# Create the library target
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# Set include directories for the library
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${glm_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        OpenGL::GL
        glfw
        libglew_static
        assimp
        nlohmann_json::nlohmann_json
)

# Set MSVC options for the library
set_msvc_options(${PROJECT_NAME})

# Create the executable target
add_executable(SnapEngineApp main.cpp)

# Link the library to the executable
target_link_libraries(SnapEngineApp PRIVATE ${PROJECT_NAME})

# Set MSVC options for the executable
set_msvc_options(SnapEngineApp)

# Copy DLLs to output directory
if(WIN32)
    add_custom_command(TARGET SnapEngineApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:assimp>
            $<TARGET_FILE_DIR:SnapEngineApp>
    )
endif()

# Copy test assets and shaders
add_custom_command(TARGET SnapEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test_assets"
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/test_assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/test_data.json"
        "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/test_data.json"
)

# Enable testing
enable_testing()
add_test(NAME SnapEngineAllTests
         COMMAND SnapEngineApp --test
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

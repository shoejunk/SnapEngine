cmake_minimum_required(VERSION 3.20)
project(SnapEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# Force all libraries to be built statically (no .dll)
# -------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries as static" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Disable Bullet3 unit tests" FORCE)

include(cmake/CPM.cmake)

# Add all dependencies using CPM
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

CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
)

CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 0.9.9.8
)

CPMAddPackage(
    NAME bgfx
    GITHUB_REPOSITORY bkaradzic/bgfx
    GIT_TAG master
    OPTIONS
        "BGFX_BUILD_EXAMPLES=OFF"
        "BGFX_BUILD_TOOLS=OFF"
)

# Box2D
CPMAddPackage(
    NAME box2d
    GITHUB_REPOSITORY erincatto/box2d
    VERSION 3.0.0
)

# Bullet Physics
CPMAddPackage(
    NAME bullet3
    GITHUB_REPOSITORY bulletphysics/bullet3
    GIT_TAG 3.25
    OPTIONS
        "BUILD_UNIT_TESTS=OFF"
)

# Dear ImGui
CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    VERSION 1.91.7
)

# SoLoud
CPMAddPackage(
    NAME soloud
    GITHUB_REPOSITORY jarikomppa/soloud
    GIT_TAG RELEASE_20200207
    OPTIONS
        "BUILD_STATIC=ON"        # Build as a static library
        "BUILD_SHARED=OFF"       # Avoid shared library builds
        "BUILD_EXAMPLES=OFF"     # Disable example builds
        "BUILD_TESTS=OFF"        # Disable test builds
        "BUILD_BACKENDS=ON"      # Enable audio backends
)

CPMAddPackage(
    NAME DirectXTK
    GITHUB_REPOSITORY microsoft/DirectXTK
    GIT_TAG main
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
	${DirectXTK_SOURCE_DIR}/Inc
)

target_link_libraries(SnapEngine
    PUBLIC
        nlohmann_json
        assimp
        stb
        glm
        bgfx
        box2d
        bullet3
        imgui
        soloud_static
        d3d11
        dxgi
        dxguid
)

# -------------------------------------------------------------------
# Compile shaders using compile_shaders.bat
# -------------------------------------------------------------------
set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")

# Create directory immediately during configuration phase
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")

add_custom_command(
    OUTPUT 
        "${SHADER_OUTPUT_DIR}/BasicVertexShader.cso"
        "${SHADER_OUTPUT_DIR}/BasicPixelShader.cso"
    COMMAND compile_shaders.bat "${SHADER_OUTPUT_DIR}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling HLSL shaders into .cso files"
    VERBATIM
)

# Create a custom target that depends on the shader files
add_custom_target(CompileShaders ALL
    DEPENDS 
        "${SHADER_OUTPUT_DIR}/BasicVertexShader.cso"
        "${SHADER_OUTPUT_DIR}/BasicPixelShader.cso"
)

# -------------------------------------------------------------------
# Create the main executable
# -------------------------------------------------------------------
add_executable(SnapEngineApp main.cpp)
add_dependencies(SnapEngineApp CompileShaders)  # Ensure shaders compile first

target_link_libraries(SnapEngineApp
    PRIVATE
        SnapEngine
        d3d11
        dxgi
        dxguid
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

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_OUTPUT_DIR}"
        "$<TARGET_FILE_DIR:SnapEngineApp>/shaders"
)

# -------------------------------------------------------------------
# Enable Testing
# -------------------------------------------------------------------
enable_testing()
add_test(
    NAME SnapEngineAllTests
    COMMAND SnapEngineApp --test
)

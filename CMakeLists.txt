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

# STB is header-only, so we just need to download it
CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
    DOWNLOAD_ONLY YES
)

if(stb_ADDED)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()

CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 0.9.9.8
)

# Dear ImGui - Download and build as a static library
CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    VERSION 1.91.7
    DOWNLOAD_ONLY YES
)

if(imgui_ADDED)
    file(GLOB IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    )
    
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC 
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC d3d11)
endif()

CPMAddPackage(
    NAME DirectXTK
    GITHUB_REPOSITORY microsoft/DirectXTK
    GIT_TAG main
    OPTIONS
        "BUILD_XAUDIO_WIN10=OFF"
        "BUILD_TOOLS=OFF"
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
        imgui
        DirectXTK
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

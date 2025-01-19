# tests/CMakeLists.txt
# Enable testing with GoogleTest or other frameworks if necessary

# Add your test executable
add_executable(SnapEngineTests
    test_snapengine.cpp
)

# Link the SnapEngine library
target_link_libraries(SnapEngineTests PRIVATE SnapEngine)

# Enable CTest for the test executable
enable_testing()
add_test(NAME SnapEngineTests COMMAND SnapEngineTests)

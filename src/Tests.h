#pragma once

/**
 * \namespace Tests
 * \brief Contains the test infrastructure for SnapEngine.
 * 
 * This namespace provides a centralized testing framework for all SnapEngine components.
 * It handles OpenGL context creation for graphics tests and ensures proper cleanup.
 * Tests can be run either through CTest or by passing --test to the main executable.
 */
namespace Tests {
    /**
     * \brief Run all unit tests for SnapEngine components.
     * 
     * This function:
     * 1. Creates a temporary OpenGL context for graphics tests
     * 2. Enables test mode for graphics components
     * 3. Runs all component tests
     * 4. Cleans up resources and test mode settings
     * 
     * \return True if all tests pass, false if any test fails or setup fails.
     */
    bool RunAllTests();

} // namespace Tests

#include "Tests.h"
#include <iostream>
#include <cassert>
#include <memory>

#include "Window.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"
#include "Shader.h"

namespace Tests {

bool RunAllTests()
{
    try
    {
        // Set test mode for all components
        Window::SetTestMode(true);
        Scene::SetTestMode(true);
        Model::SetTestMode(true);  // Add this line

        // Run component tests
        std::cout << "\nRunning Model tests...\n";
        Model::test();

        std::cout << "\nRunning Window tests...\n";
        Window::test();

        std::cout << "\nRunning WindowManager tests...\n";
        WindowManager::test();

        std::cout << "\nRunning DataManager tests...\n";
        DataManager::test();

        std::cout << "\nRunning Scene tests...\n";
        Scene::test();

        std::cout << "\nRunning Camera tests...\n";
        Camera::test();

        std::cout << "\nRunning Shader tests...\n";
        Shader::test();

        std::cout << "\nAll tests passed!\n";
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return false;
    }
}

} // namespace Tests

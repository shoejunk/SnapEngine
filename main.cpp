#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"

int main(int argc, char* argv[])
{
    bool runTests = false;

    // Check for --test argument
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--test")
        {
            runTests = true;
            break;
        }
    }

    if (runTests)
    {
        std::cout << "Running tests..." << std::endl;
        Window::test();
        WindowManager::test();
        DataManager::test();
        return 0;
    }

    try
    {
        // Create and initialize the data manager
        DataManager dataManager;
        if (!dataManager.Initialize())
        {
            std::cerr << "Failed to initialize data manager" << std::endl;
            return 1;
        }

        // Get window manager and initialize it first
        WindowManager& windowManager = dataManager.GetWindowManager();
        if (!windowManager.Initialize())
        {
            std::cerr << "Failed to initialize window manager" << std::endl;
            return 1;
        }

        // Load JSON data
        if (!dataManager.LoadData())
        {
            std::cerr << "Failed to load data" << std::endl;
            return 1;
        }

        // Create all managed objects from the loaded data
        dataManager.CreateManagedObjects();

        // Main loop
        while (windowManager.ProcessMessages())
        {
            // TODO: Add rendering code here
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

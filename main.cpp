#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // Include nlohmann/json if not already
#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"

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
        std::cout << "Running SnapEngine tests...\n";

        // Test the existing Window class
        Window::test();

        // Test the new WindowManager
        WindowManager::test();

        // ... call other test() methods from other classes if needed

        std::cout << "All tests done.\n";
    }
    else
    {
        // Normal mode: Use WindowManager to create/manage windows from JSON data
        std::cout << "Welcome to SnapEngine!" << std::endl;

        // Create a WindowManager
        WindowManager windowManager;

        // Example JSON describing one or more windows
        nlohmann::json mainWindowJson = {
            {"title",  "My SnapEngine Window (Managed)"},
            {"width",  800},
            {"height", 600}
        };

        // Add the JSON object to WindowManager
        windowManager.addJsonObject(mainWindowJson);

        // Create all windows based on the stored JSON
        try
        {
            windowManager.createObjects();
        }
        catch(const std::exception& e)
        {
            std::cerr << "Failed to create window(s): " << e.what() << std::endl;
            return -1;
        }

        // Main loop
        bool running = true;
        while (running)
        {
            // Process messages for each window
            for (auto& wPtr : windowManager.GetWindows())
            {
                if (!wPtr->ProcessMessages())
                {
                    running = false;
                    break;
                }
            }

            // TODO: Place your update/render calls here
        }

        std::cout << "Exiting SnapEngine." << std::endl;
    }

    return 0;
}

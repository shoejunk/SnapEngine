#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // Include nlohmann/json if not already

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h" // <-- Include Model header here

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

        // Test the Window class
        Window::test();

        // Test the WindowManager
        WindowManager::test();

        // Test the DataManager
        DataManager::test();

        // Test the Model class
        Model::test();

        std::cout << "All tests done.\n";
    }
    else
    {
        // Normal mode: use DataManager to load JSON data, then create windows
        std::cout << "Welcome to SnapEngine!" << std::endl;

        // Create a DataManager with the JSON file we want to load
        DataManager dataManager("snapengine_data.json");
        if (!dataManager.LoadData())
        {
            std::cerr << "Failed to load data from snapengine_data.json\n";
            return -1;
        }

        // Ask DataManager to create all objects (e.g., windows)
        try
        {
            dataManager.CreateManagedObjects();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to create objects: " << e.what() << std::endl;
            return -1;
        }

        // Get the WindowManager from the DataManager
        WindowManager& windowManager = dataManager.GetWindowManager();

        // Load a 3D model (box.obj) via our Model class
        Model myModel;
        if (!myModel.LoadFromFile("test_assets/box.obj"))
        {
            std::cerr << "Failed to load 'test_assets/box.obj'." << std::endl;
        }
        else
        {
            std::cout << "Successfully loaded 'test_assets/box.obj'!" << std::endl;
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

            // In a real engine, you'd call your rendering code here each frame.
            // For now, we'll just call myModel.Draw() as a placeholder:
            myModel.Draw();

            // TODO: Place your update/render calls here
            // e.g., handle input, update game objects, etc.
        }

        std::cout << "Exiting SnapEngine." << std::endl;
    }

    return 0;
}

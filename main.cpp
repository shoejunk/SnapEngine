#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // Include nlohmann/json if not already

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"

// Include your concrete renderer implementation, e.g. D3D11Renderer
#include "D3D11Renderer.h" 

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

        // If you have test() in your renderer classes:
        // RendererBase::test();
        // D3D11Renderer::test();

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
        auto& windows = windowManager.GetWindows();

        if (windows.empty())
        {
            std::cerr << "No windows created; cannot continue.\n";
            return -1;
        }

        // We'll just use the first window for rendering
        Window* mainWindow = windows.front().get();
        if (!mainWindow)
        {
            std::cerr << "Window pointer is null; cannot continue.\n";
            return -1;
        }

        // Create a D3D11Renderer (or any other derived RendererBase you have)
        D3D11Renderer renderer;
        // Initialize it with the Win32 HWND from your Window class
        if (!renderer.Initialize(static_cast<void*>(mainWindow->GetHandle())))
        {
            std::cerr << "Failed to initialize the D3D11Renderer.\n";
            return -1;
        }

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
            for (auto& wPtr : windows)
            {
                if (!wPtr->ProcessMessages())
                {
                    running = false;
                    break;
                }
            }

            // ------ Rendering step ------
            renderer.BeginFrame();

            // Draw the model
            renderer.DrawModel(myModel);

            // Present the frame
            renderer.EndFrame();

            // ---------------------------------------
            // TODO: Place your update logic here
            // e.g., handle input, update game objects, etc.
        }

        std::cout << "Exiting SnapEngine." << std::endl;
    }

    return 0;
}

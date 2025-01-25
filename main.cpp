#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // Include nlohmann/json if not already

#include "Window.h"
#include "ManagerBase.h"
#include "WindowManager.h"
#include "DataManager.h"
#include "Model.h"
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

        // Test the Renderer classes
        RendererBase::test();
        D3D11Renderer::test();

        std::cout << "All tests done.\n";
    }
    else
    {
        std::cout << "Welcome to SnapEngine!\n";

        // Create a DataManager with the JSON file we want to load
        DataManager dataManager("snapengine_data.json");
        if (!dataManager.LoadData())
        {
            std::cerr << "Failed to load data from snapengine_data.json\n";
            return -1;
        }

        // Create all objects (e.g., windows)
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

        // We'll use the first window for rendering
        Window* mainWindow = windows.front().get();
        if (!mainWindow)
        {
            std::cerr << "Window pointer is null; cannot continue.\n";
            return -1;
        }

        // Create and initialize a D3D11Renderer
        D3D11Renderer renderer;
        if (!renderer.Initialize(static_cast<void*>(mainWindow->GetHandle())))
        {
            std::cerr << "Failed to initialize the D3D11Renderer.\n";
            return -1;
        }

        // Load a 3D model (box.obj) via the Model class
        Model myModel;
        if (myModel.LoadFromFile("test_assets/box.obj"))
        {
            std::cout << "Successfully loaded 'test_assets/box.obj'.\n";
            std::cout << "Model contains " << myModel.GetMeshCount() << " meshes.\n";

            for (size_t i = 0; i < myModel.GetMeshCount(); ++i)
            {
                const auto& mesh = myModel.GetMeshes()[i];
                std::cout << "Mesh " << i << ": " << mesh.vertices.size() / 8 << " vertices, "
                        << mesh.indices.size() << " indices.\n";
            }
        }
        else
        {
            std::cerr << "Failed to load 'test_assets/box.obj'.\n";
            return -1;
        }

        // Main loop
        bool running = true;
        while (running)
        {
            // Process window messages
            for (auto& wPtr : windows)
            {
                if (!wPtr->ProcessMessages())
                {
                    running = false;
                    break;
                }
            }

            // Begin rendering
            renderer.BeginFrame();

            // Draw the loaded model
            if (myModel.GetMeshCount() > 0)
            {
                renderer.DrawModel(myModel);
            }
            else
            {
                std::cerr << "Model has no meshes to draw.\n";
            }

            // End rendering
            renderer.EndFrame();

            // Additional logic can be added here
        }

        std::cout << "Exiting SnapEngine.\n";
    }

    return 0;
}
